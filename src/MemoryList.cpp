#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cassert>

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
blockPtr MemoryList::createMemoryBlock(U8* pStart, bool isFree, size_t size)
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
	m_pListStart = reinterpret_cast<blockPtr>(pMemStart);
	*m_pListStart = listStartMeta;
	U8* pMemEnd = pMemStart + size;
	m_pListEnd = reinterpret_cast<blockPtr>(pMemEnd - sizeof(metadata_type));
	*m_pListEnd = listEndMeta;
	U8* pFirstBlockStart = pMemStart + sizeof(metadata_type);
	size_t firstBlockSize = size - 2 * sizeof(metadata_type);
	createMemoryBlock(pFirstBlockStart, true, firstBlockSize);
}

bool MemoryList::isFree(const blockPtr pMetadata)
{
	return (*pMetadata & 1) > 0;
}

size_t MemoryList::getSize(const blockPtr pMetadata)
{
	return *pMetadata >> 1;
}

blockPtr MemoryList::getPrevBlock(blockPtr pBlock)
{
	assert(!isStartOfList(pBlock));

	// Prev block's footer must be directly before this block's header
	metadata_type* pPrevFooter = pBlock - 1;
	size_t prevBlockSize = getSize(pPrevFooter);

	// Once we know size of previous block, roll ptr back that many bytes
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlock);
	U8* pPrevBlockStart = pThisBlockStart - prevBlockSize;
	blockPtr pResult = reinterpret_cast<blockPtr>(pPrevBlockStart);
	return pResult;
}

blockPtr MemoryList::getNextBlock(blockPtr pBlock)
{
	assert(!isEndOfList(pBlock));

	// Move ptr forward based on how big this block is
	size_t thisBlockSize = getSize(pBlock);
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlock);
	U8* pNextBlockStart = pThisBlockStart + thisBlockSize;
	metadata_type* pResult = reinterpret_cast<metadata_type*>(pNextBlockStart);
	return pResult;
}


bool MemoryList::isStartOfList(const blockPtr bBlock)
{
	// Special case: Size is sizeOf(metadata_type) and isFree flag off
	return *bBlock == listStartMeta;
}

bool MemoryList::isEndOfList(const blockPtr bBlock)
{
	// Special case: Size sizeOf(metadata_type) isFree flag on
	return *bBlock == listEndMeta;
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