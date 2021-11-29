#include "DynamicListAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "MemoryUtils.h"

namespace GameMemorySystem
{

void DynamicListAllocator::init(U8* pMemStart, size_t bytes)
{
	m_pMem = pMemStart;
	m_size = bytes;
	m_pListStart = reinterpret_cast<blockMetadata_t*>(m_pMem);
	*m_pListStart = listStartMeta;
	U8* pMemEnd = m_pMem + m_size;
	m_pListEnd = reinterpret_cast<blockMetadata_t*>(pMemEnd - sizeof(blockMetadata_t));
	*m_pListEnd = listEndMeta;
	U8* pFirstBlockStart = m_pMem + sizeof(blockMetadata_t);
	size_t firstBlockSize = m_size - 2 * sizeof(blockMetadata_t);
	writeBlock(pFirstBlockStart, true, firstBlockSize);
}

void DynamicListAllocator::print() const
{
	std::cout << "MemHeap size: " << std::dec << m_size << std::endl;
	for (size_t i = 0; i < m_size; ++i)
	{
		std::cout << std::hex << (size_t)m_pMem + i << std::dec << ":\n";
	}
}

void* DynamicListAllocator::alloc(size_t size, Alignment align)
{
	// Look for smallest block that meets our size requirement
	// TODO: Make heap to do this
	size_t requiredBlockSize = getMaxPayloadSize(size, align) + 2 * sizeof(blockMetadata_t);
	size_t smallestSuitableSize = std::numeric_limits<size_t>::max();
	blockMetadata_t* pMostSuitableBlock = nullptr;
	for (blockMetadata_t* pBlockHeader : *this)
	{
		size_t blockSize = getSize(pBlockHeader);
		if (blockSize < smallestSuitableSize && blockSize >= requiredBlockSize && isFree(pBlockHeader))
		{
			pMostSuitableBlock = pBlockHeader;
			smallestSuitableSize = blockSize;
		}
	}

	// Make sure we found a suitable block
	if (pMostSuitableBlock == nullptr)
	{
		throw std::bad_alloc();
	}

	// Check if block should be split
	if (getSize(pMostSuitableBlock) - requiredBlockSize >= minBlockSize)
	{
		splitBlock(pMostSuitableBlock, requiredBlockSize);
	}

	// Calc & write offset for alignment
	U8* pPayloadStart = reinterpret_cast<U8*>(pMostSuitableBlock) + sizeof(blockMetadata_t);
	U8* pAlignedStart = AlignWithSpaceForPrefix(pPayloadStart, align);
	U8 offset = pAlignedStart - pPayloadStart;
	pPayloadStart[0] = offset;
	pAlignedStart[-1] = offset;

	// Change the free bit
	setNotFree(pMostSuitableBlock);

	return pAlignedStart;
}

void DynamicListAllocator::free(void* ptr)
{

}

DynamicListAllocator::iterator& DynamicListAllocator::iterator::operator++()
{
	m_pBlockHeader = getNextBlock(m_pBlockHeader);
	return *this;
}

DynamicListAllocator::iterator DynamicListAllocator::iterator::operator++(int)
{
	iterator temp = *this;
	++* this;
	return temp;
}

DynamicListAllocator::iterator& DynamicListAllocator::iterator::operator--()
{
	m_pBlockHeader = getPrevBlock(m_pBlockHeader);
	return *this;
}

DynamicListAllocator::iterator DynamicListAllocator::iterator::operator--(int)
{
	iterator temp = *this;
	--* this;
	return temp;
}

// Writes the given metadata into the given location
void DynamicListAllocator::writeMetadata(blockMetadata_t* pLocation, bool isFree, size_t size)
{
	// Ensure that size does not exceed storage space in header
	// One bit stores "Free" flag so we have sizeof(blockMetadata_t) * 8 - 1 bits
	blockMetadata_t maxSize = -1;
	maxSize = maxSize >> 1;
	assert(size <= maxSize);

	// Combine isFree and size and write result into desired location
	blockMetadata_t isFree_bits = static_cast<blockMetadata_t>(isFree);
	blockMetadata_t size_bits = static_cast<blockMetadata_t>(size) << 1;
	*pLocation = isFree_bits | size_bits;
}

// Writes metadata that defines the start and end of a memory block.
// Returns ptr to the header of the block
blockMetadata_t* DynamicListAllocator::writeBlock(U8* pStart, bool isFree, size_t size)
{
	assert(size >= minBlockSize);

	blockMetadata_t* pHeader = reinterpret_cast<blockMetadata_t*>(pStart);
	writeMetadata(pHeader, isFree, size);
	U8* end = pStart + size;
	blockMetadata_t* pFooter = reinterpret_cast<blockMetadata_t*>(end - sizeof(blockMetadata_t));
	writeMetadata(pFooter, isFree, size);
	return pHeader;
}

void DynamicListAllocator::splitBlock(blockMetadata_t* pBlockHeader, size_t sizeOfFirstNewBlock)
{
	size_t totalSize = getSize(pBlockHeader);
	size_t sizeOfSecondNewBlock = totalSize - sizeOfFirstNewBlock;

	assert(isFree(pBlockHeader));
	assert(sizeOfFirstNewBlock >= minBlockSize);
	assert(sizeOfSecondNewBlock >= minBlockSize);

	U8* pFirstBlockStart = reinterpret_cast<U8*>(pBlockHeader);
	writeBlock(pFirstBlockStart, true, sizeOfFirstNewBlock);
	U8* pSecondBlockStart = pFirstBlockStart + sizeOfFirstNewBlock;
	writeBlock(pSecondBlockStart, true, sizeOfSecondNewBlock);
}

bool DynamicListAllocator::isFree(const blockMetadata_t* pMetadata)
{
	return (*pMetadata & 1) > 0;
}

size_t DynamicListAllocator::getSize(const blockMetadata_t* pMetadata)
{
	return *pMetadata >> 1;
}

blockMetadata_t* DynamicListAllocator::getPrevBlock(blockMetadata_t* pBlockHeader)
{
	assert(!isStartOfList(pBlockHeader));

	// Prev block's footer must be directly before this block's header
	blockMetadata_t* pPrevFooter = pBlockHeader - 1;
	size_t prevBlockSize = getSize(pPrevFooter);

	// Once we know size of previous block, roll ptr back that many bytes
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlockHeader);
	U8* pPrevBlockStart = pThisBlockStart - prevBlockSize;
	blockMetadata_t* pResult = reinterpret_cast<blockMetadata_t*>(pPrevBlockStart);
	return pResult;
}

blockMetadata_t* DynamicListAllocator::getNextBlock(blockMetadata_t* pBlockHeader)
{
	assert(!isEndOfList(pBlockHeader));

	// Move ptr forward based on how big this block is
	size_t thisBlockSize = getSize(pBlockHeader);
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlockHeader);
	U8* pNextBlockStart = pThisBlockStart + thisBlockSize;
	blockMetadata_t* pResult = reinterpret_cast<blockMetadata_t*>(pNextBlockStart);
	return pResult;
}


bool DynamicListAllocator::isStartOfList(const blockMetadata_t* pBlockHeader)
{
	return *pBlockHeader == listStartMeta;
}

bool DynamicListAllocator::isEndOfList(const blockMetadata_t* pBlockHeader)
{
	return *pBlockHeader == listEndMeta;
}

blockMetadata_t* DynamicListAllocator::getFooter(blockMetadata_t* pBlockHeader)
{
	size_t size = getSize(pBlockHeader);
	U8* pTemp = reinterpret_cast<U8*>(pBlockHeader);
	pTemp += size;
	pTemp -= sizeof(blockMetadata_t);
	blockMetadata_t* result = reinterpret_cast<blockMetadata_t*>(pTemp);
	return result;
}

void DynamicListAllocator::setFree(blockMetadata_t* pBlockHeader)
{
	*pBlockHeader |= 1;
	blockMetadata_t* pFooter = getFooter(pBlockHeader);
	*pFooter |= 1;
}

void DynamicListAllocator::setNotFree(blockMetadata_t* pBlockHeader)
{
	*pBlockHeader &= ~1;
	blockMetadata_t* pFooter = getFooter(pBlockHeader);
	*pFooter &= ~1;
}

DynamicListAllocator::iterator DynamicListAllocator::begin()
{
	return iterator(getNextBlock(m_pListStart));
}

DynamicListAllocator::iterator DynamicListAllocator::end()
{
	return iterator(m_pListEnd);
}

}