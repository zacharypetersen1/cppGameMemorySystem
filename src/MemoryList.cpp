#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cassert>
#include <limits>
#include <new>

namespace GameMemorySystem
{

MemoryList::iterator& MemoryList::iterator::operator++()
{
	m_pBlockPtr = getNextBlock(m_pBlockPtr);
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
	m_pBlockPtr = getPrevBlock(m_pBlockPtr);
	return *this;
}

MemoryList::iterator MemoryList::iterator::operator--(int)
{
	iterator temp = *this;
	--* this;
	return temp;
}

// Writes the given metadata into the given location
void MemoryList::writeMetadata(metadata_type* pLocation, bool isFree, size_t size)
{
	// Ensure that size does not exceed storage space in header
	// One bit stores "Free" flag so we have sizeof(metadata_type) * 8 - 1 bits
	metadata_type maxSize = -1;
	maxSize = maxSize >> 1;
	assert(size <= maxSize);

	// Combine isFree and size and write result into desired location
	metadata_type isFree_bits = static_cast<metadata_type>(isFree);
	metadata_type size_bits = static_cast<metadata_type>(size) << 1;
	*pLocation = isFree_bits | size_bits;
}

// Writes metadata that defines the start and end of a memory block.
// Returns ptr to the header of the block
blockHeaderPtr MemoryList::writeBlock(U8* pStart, bool isFree, size_t size)
{
	assert(size >= minBlockSize);

	metadata_type* pHeader = reinterpret_cast<metadata_type*>(pStart);
	writeMetadata(pHeader, isFree, size);
	U8* end = pStart + size;
	metadata_type* pFooter = reinterpret_cast<metadata_type*>(end - sizeof(metadata_type));
	writeMetadata(pFooter, isFree, size);
	return pHeader;
}

void MemoryList::init(U8* pMemStart, size_t size)
{
	m_pListStart = reinterpret_cast<blockHeaderPtr>(pMemStart);
	*m_pListStart = listStartMeta;
	U8* pMemEnd = pMemStart + size;
	m_pListEnd = reinterpret_cast<blockHeaderPtr>(pMemEnd - sizeof(metadata_type));
	*m_pListEnd = listEndMeta;
	U8* pFirstBlockStart = pMemStart + sizeof(metadata_type);
	size_t firstBlockSize = size - 2 * sizeof(metadata_type);
	writeBlock(pFirstBlockStart, true, firstBlockSize);
}

void MemoryList::splitBlock(blockHeaderPtr pBlockToSplit, size_t sizeOfFirstBlock)
{
	size_t totalSize = getSize(pBlockToSplit);
	size_t sizeOfSecondBlock = totalSize - sizeOfFirstBlock;

	assert(isFree(pBlockToSplit));
	assert(sizeOfSecondBlock >= minBlockSize);

	U8* pFirstBlockStart = reinterpret_cast<U8*>(pBlockToSplit);
	writeBlock(pFirstBlockStart, true, sizeOfFirstBlock);
	U8* pSecondBlockStart = pFirstBlockStart + sizeOfFirstBlock;
	writeBlock(pSecondBlockStart, true, sizeOfSecondBlock);
}

void* MemoryList::alloc(size_t size, Alignment alignment)
{
	// Look for smallest block that meets our size requirement
	// TODO: Make heap to do this
	size_t requiredBlockSize = getMaxPayloadSize(size, alignment) + 2 * sizeof(metadata_type);
	size_t smallestSuitableSize = std::numeric_limits<size_t>::max();
	blockHeaderPtr pMostSuitableBlock = nullptr;
	for (blockHeaderPtr pBlock : *this)
	{
		size_t blockSize = getSize(pBlock);
		if (blockSize < smallestSuitableSize && blockSize >= requiredBlockSize && isFree(pBlock))
		{
			pMostSuitableBlock = pBlock;
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
	U8* pPayloadStart = reinterpret_cast<U8*>(pMostSuitableBlock) + sizeof(metadata_type);
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

bool MemoryList::isFree(const blockHeaderPtr pMetadata)
{
	return (*pMetadata & 1) > 0;
}

size_t MemoryList::getSize(const blockHeaderPtr pMetadata)
{
	return *pMetadata >> 1;
}

blockHeaderPtr MemoryList::getPrevBlock(blockHeaderPtr pBlock)
{
	assert(!isStartOfList(pBlock));

	// Prev block's footer must be directly before this block's header
	metadata_type* pPrevFooter = pBlock - 1;
	size_t prevBlockSize = getSize(pPrevFooter);

	// Once we know size of previous block, roll ptr back that many bytes
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlock);
	U8* pPrevBlockStart = pThisBlockStart - prevBlockSize;
	blockHeaderPtr pResult = reinterpret_cast<blockHeaderPtr>(pPrevBlockStart);
	return pResult;
}

blockHeaderPtr MemoryList::getNextBlock(blockHeaderPtr pBlock)
{
	assert(!isEndOfList(pBlock));

	// Move ptr forward based on how big this block is
	size_t thisBlockSize = getSize(pBlock);
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlock);
	U8* pNextBlockStart = pThisBlockStart + thisBlockSize;
	metadata_type* pResult = reinterpret_cast<metadata_type*>(pNextBlockStart);
	return pResult;
}


bool MemoryList::isStartOfList(const blockHeaderPtr bBlock)
{
	// Special case: Size is sizeOf(metadata_type) and isFree flag off
	return *bBlock == listStartMeta;
}

bool MemoryList::isEndOfList(const blockHeaderPtr bBlock)
{
	// Special case: Size sizeOf(metadata_type) isFree flag on
	return *bBlock == listEndMeta;
}

metadata_type* MemoryList::getFooter(blockHeaderPtr pBlock)
{
	size_t size = getSize(pBlock);
	U8* pTemp = reinterpret_cast<U8*>(pBlock);
	pTemp += size;
	pTemp -= sizeof(metadata_type);
	metadata_type* result = reinterpret_cast<metadata_type*>(pTemp);
	return result;
}

void MemoryList::setFree(blockHeaderPtr pBlock)
{
	*pBlock |= 1;
	metadata_type* pFooter = getFooter(pBlock);
	*pFooter |= 1;
}

void MemoryList::setNotFree(blockHeaderPtr pBlock)
{
	*pBlock &= ~1;
	metadata_type* pFooter = getFooter(pBlock);
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