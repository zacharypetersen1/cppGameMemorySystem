#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{

// Type used for storing metadata in the header/footer of memory block
// Rightmost bit contains isFree flag, all other bits contain size.
typedef size_t metadata_type;

// a blockPtr points to the header metadata of the block
typedef metadata_type* blockPtr;

// Writes metadata that defines the start and end of a memory block.
// Returns ptr to the header of the block
blockPtr createMemoryBlock(U8* pStart, bool isFree, size_t size);

// Writes the given metadata into the given location
void writeMetadata(metadata_type* pLocation, bool isFree, size_t size);

// Returns true if the block is free
bool isFree(const metadata_type* pMetadata);

// Returns the size of the block
size_t getSize(const metadata_type* pMetadata);

// Returns header of the next block
blockPtr getPrevBlock(blockPtr pBlock);

// Returns header of the previous block
blockPtr getNextBlock(blockPtr pBlock);

}
