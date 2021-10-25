#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cassert>

namespace GameMemorySystem
{

blockPtr createMemoryBlock(U8* pStart, bool isFree, size_t size)
{
	metadata_type* pHeader = reinterpret_cast<metadata_type*>(pStart);
	writeMetadata(pHeader, isFree, size);
	U8* end = pStart + size;
	metadata_type* pFooter = reinterpret_cast<metadata_type*>(end - sizeof(metadata_type));
	writeMetadata(pFooter, isFree, size);
	return pHeader;
}

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

bool isFree(const metadata_type* pMetadata)
{
	return (*pMetadata & 1) > 0;
}

size_t getSize(const metadata_type* pMetadata)
{
	return *pMetadata >> 1;
}

blockPtr getPrevBlock(blockPtr pBlock)
{
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
	// Move ptr forward based on how big this block is
	size_t thisBlockSize = getSize(pBlock);
	U8* pThisBlockStart = reinterpret_cast<U8*>(pBlock);
	U8* pNextBlockStart = pThisBlockStart + thisBlockSize;
	metadata_type* pResult = reinterpret_cast<metadata_type*>(pNextBlockStart);
	return pResult;
}

}