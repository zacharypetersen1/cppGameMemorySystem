#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{

// Type used for storing metadata in the header/footer of memory block
// Rightmost bit contains isFree flag, all other bits contain size.
typedef size_t metadata_type;

// Points to the header metadata of a block
typedef metadata_type* blockPtr;

// Minimum size of a block in bytes
constexpr size_t minBlockSize = 2 * sizeof(metadata_type) + 2 * sizeof(blockPtr);

// Maintains a list of memory blocks
class MemoryList
{
private:
	// Ptr to the start of the list
	blockPtr m_pListStart = nullptr;
	blockPtr m_pListEnd = nullptr;

	// Special metadata values that indicate blocks lying at the start/end of list
	static constexpr metadata_type listStartMeta = sizeof(metadata_type) << 1;
	static constexpr metadata_type listEndMeta = (sizeof(metadata_type) << 1) | 1;

public:
	// Bidirectional iterator for traversing blocks of memory
	class iterator
	{
	private:
		blockPtr m_pBlockPtr;
	public:
		iterator() : m_pBlockPtr(nullptr) { }
		iterator(blockPtr pBlockPtr) : m_pBlockPtr(pBlockPtr) { }
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) { return m_pBlockPtr == other.m_pBlockPtr; }
		bool operator!=(const iterator& other) { return !(*this == other); }
		blockPtr operator*() { return m_pBlockPtr; }
		blockPtr* operator->() { return &m_pBlockPtr; }
	};

private:
	// Splits given block into two blocks.
	// Size given is the size of first new block in bytes.
	void splitBlock(blockPtr pBlock, size_t sizeOfFirstSection);

public:
	// Writes metadata that defines the start and end of a memory block.
	// Returns ptr to the header of the block
	blockPtr writeBlock(U8* pStart, bool isFree, size_t size);

	// Initializes the memory list
	void init(U8* pMemStart, size_t size);

	// Allocates new memory block that will fit size bytes with given alignment
	void* alloc(size_t size, Alignment alignment);

	// Frees the memory block that given ptr lies within
	void free(void* ptr);

	// Returns iterator to the first block
	iterator begin();

	// Returns iterator to block after the last block
	iterator end();

	// Writes the given metadata into the given location
	static void writeMetadata(metadata_type* pLocation, bool isFree, size_t size);

	// Returns true if the block is free
	static bool isFree(const blockPtr pMetadata);

	// Returns the size of the block
	static size_t getSize(const blockPtr pMetadata);

	// Returns header of the next block
	static blockPtr getPrevBlock(blockPtr pBlock);

	// Returns header of the previous block
	static blockPtr getNextBlock(blockPtr pBlock);

	// Returns true if given block is the special "start" block
	static bool isStartOfList(const blockPtr bBlock);

	// Returns true if given block is the special "end" block
	static bool isEndOfList(const blockPtr bBlock);
};

}
