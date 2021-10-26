#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{

// Type used for storing metadata in the header/footer of memory block
// Rightmost bit contains isFree flag, all other bits contain size.
typedef size_t metadata_type;

// a blockPtr points to the header metadata of the block
typedef metadata_type* blockPtr;

// Minimum size of a memory block in bytes
constexpr size_t minBlockSize = 4 * sizeof(metadata_type);

// Creates a memory list.
// Returns memory block spanning the entire usable chunk of memory.
blockPtr createMemoryList(U8* pMemStart, size_t size);

// Returns true if the block is free
bool isFree(const blockPtr pMetadata);

// Returns the size of the block
size_t getSize(const blockPtr pMetadata);

// Returns header of the next block
blockPtr getPrevBlock(blockPtr pBlock);

// Returns header of the previous block
blockPtr getNextBlock(blockPtr pBlock);

// Returns true if given block is the first block
bool isStartOfList(const blockPtr bBlock);

// Returns true if given block is the last block
bool isEndOfList(const blockPtr bBlock);

}
