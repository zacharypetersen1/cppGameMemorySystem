#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cassert>
#include <limits>
#include <new>

namespace GameMemorySystem
{

MemoryList::iterator& MemoryList::iterator::operator++()
{
	m_pBlockHeader = getNextBlock(m_pBlockHeader);
	return *this;
}

MemoryList::iterator MemoryList::iterator::operator++(int)
{
	iterator temp = *this;
	++* this;
	return temp;
}

MemoryList::iterator& MemoryList::iterator::operator--()
{
	m_pBlockHeader = getPrevBlock(m_pBlockHeader);
	return *this;
}

MemoryList::iterator MemoryList::iterator::operator--(int)
{
	iterator temp = *this;
	--* this;
	return temp;
}

// Writes the given metadata into the given location
void MemoryList::writeMetadata(blockMetadata_t* pLocation, bool isFree, size_t size)
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
blockMetadata_t* MemoryList::writeBlock(U8* pStart, bool isFree, size_t size)
{
	assert(size >= minBlockSize);

	blockMetadata_t* pHeader = reinterpret_cast<blockMetadata_t*>(pStart);
	writeMetadata(pHeader, isFree, size);
	U8* end = pStart + size;
	blockMetadata_t* pFooter = reinterpret_cast<blockMetadata_t*>(end - sizeof(blockMetadata_t));
	writeMetadata(pFooter, isFree, size);
	return pHeader;
}

void MemoryList::init(U8* pMemStart, size_t size)
{
	m_pListStart = reinterpret_cast<blockMetadata_t*>(pMemStart);
	*m_pListStart = listStartMeta;
	U8* pMemEnd = pMemStart + size;
	m_pListEnd = reinterpret_cast<blockMetadata_t*>(pMemEnd - sizeof(blockMetadata_t));
	*m_pListEnd = listEndMeta;
	U8* pFirstBlockStart = pMemStart + sizeof(blockMetadata_t);
	size_t firstBlockSize = size - 2 * sizeof(blockMetadata_t);
	writeBlock(pFirstBlockStart, true, firstBlockSize);
}

void MemoryList::splitBlock(blockMetadata_t* pBlockHeader, size_t sizeOfFirstNewBlock)
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

void* MemoryList::alloc(size_t size, Alignment alignment)
{
	// Look for smallest block that meets our size requirement
	// TODO: Make heap to do this
	size_t requiredBlockSize = getMaxPayloadSize(size, alignment) + 2 * sizeof(blockMetadata_t);
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
	U8* pAlignedStart = AlignWithSpaceForPrefix(pPayloadStart, alignment);
	U8 offset = pAlignedStart - pPayloadStart;
	pPayloadStart[0] = offset;
	pAlignedStart[-1] = offset;

	// Change the free bit
	setNotFree(pMostSuitableBlock);

	return pAlignedStart;
}

void MemoryList::free(void* ptr)
{

}

bool MemoryList::isFree(const blockMetadata_t* pMetadata)
{
	return (*pMetadata & 1) > 0;
}

size_t MemoryList::getSize(const blockMetadata_t* pMetadata)
{
	return *pMetadata >> 1;
}

blockMetadata_t* MemoryList::getPrevBlock(blockMetadata_t* pBlockHeader)
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

blockMetadata_t* MemoryList::getNextBlock(blockMetadata_t* pBlockHeader)
{
	assert(!isEndOfList(pBlockHeader));

	// Move ptr forward based on how big this block is
	size_t thisBlockSize = getSize(pBlockHeader);
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlockHeader);
	U8* pNextBlockStart = pThisBlockStart + thisBlockSize;
	blockMetadata_t* pResult = reinterpret_cast<blockMetadata_t*>(pNextBlockStart);
	return pResult;
}


bool MemoryList::isStartOfList(const blockMetadata_t* pBlockHeader)
{
	return *pBlockHeader == listStartMeta;
}

bool MemoryList::isEndOfList(const blockMetadata_t* pBlockHeader)
{
	return *pBlockHeader == listEndMeta;
}

blockMetadata_t* MemoryList::getFooter(blockMetadata_t* pBlockHeader)
{
	size_t size = getSize(pBlockHeader);
	U8* pTemp = reinterpret_cast<U8*>(pBlockHeader);
	pTemp += size;
	pTemp -= sizeof(blockMetadata_t);
	blockMetadata_t* result = reinterpret_cast<blockMetadata_t*>(pTemp);
	return result;
}

void MemoryList::setFree(blockMetadata_t* pBlockHeader)
{
	*pBlockHeader |= 1;
	blockMetadata_t* pFooter = getFooter(pBlockHeader);
	*pFooter |= 1;
}

void MemoryList::setNotFree(blockMetadata_t* pBlockHeader)
{
	*pBlockHeader &= ~1;
	blockMetadata_t* pFooter = getFooter(pBlockHeader);
	*pFooter &= ~1;
}

MemoryList::iterator MemoryList::begin()
{
	return iterator(getNextBlock(m_pListStart));
}

MemoryList::iterator MemoryList::end()
{
	return iterator(m_pListEnd);
}

}