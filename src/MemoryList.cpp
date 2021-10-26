#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cassert>

namespace GameMemorySystem
{

constexpr metadata_type listStartMeta = sizeof(metadata_type) << 1;
constexpr metadata_type listEndMeta = (sizeof(metadata_type) << 1) | 1;

// Writes the given metadata into the given location
void writeMetadata(metadata_type* pLocation, bool isFree, size_t size)
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
blockPtr createMemoryBlock(U8* pStart, bool isFree, size_t size)
{
	assert(size >= minBlockSize);

	metadata_type* pHeader = reinterpret_cast<metadata_type*>(pStart);
	writeMetadata(pHeader, isFree, size);
	U8* end = pStart + size;
	metadata_type* pFooter = reinterpret_cast<metadata_type*>(end - sizeof(metadata_type));
	writeMetadata(pFooter, isFree, size);
	return pHeader;
}

blockPtr createMemoryList(U8* pMemStart, size_t size)
{
	metadata_type* pListStart = reinterpret_cast<metadata_type*>(pMemStart);
	*pListStart = listStartMeta;
	U8* pMemEnd = pMemStart + size;
	metadata_type* pListEnd = reinterpret_cast<metadata_type*>(pMemEnd - sizeof(metadata_type));
	*pListEnd = listEndMeta;
	U8* pFirstBlockStart = pMemStart + sizeof(metadata_type);
	size_t firstBlockSize = size - 2 * sizeof(metadata_type);
	blockPtr pFirstBlock = createMemoryBlock(pFirstBlockStart, true, firstBlockSize);
	return pFirstBlock;
}

bool isFree(const blockPtr pMetadata)
{
	return (*pMetadata & 1) > 0;
}

size_t getSize(const blockPtr pMetadata)
{
	return *pMetadata >> 1;
}

blockPtr getPrevBlock(blockPtr pBlock)
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

blockPtr getNextBlock(blockPtr pBlock)
{
	assert(!isEndOfList(pBlock));

	// Move ptr forward based on how big this block is
	size_t thisBlockSize = getSize(pBlock);
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlock);
	U8* pNextBlockStart = pThisBlockStart + thisBlockSize;
	metadata_type* pResult = reinterpret_cast<metadata_type*>(pNextBlockStart);
	return pResult;
}


bool isStartOfList(const blockPtr bBlock)
{
	// Special case: Size is sizeOf(metadata_type) and isFree flag off
	return *bBlock == listStartMeta;
}

bool isEndOfList(const blockPtr bBlock)
{
	// Special case: Size sizeOf(metadata_type) isFree flag on
	return *bBlock == listEndMeta;
}

}