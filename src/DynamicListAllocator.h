#pragma once
#include "MemoryUtils.h"
#include "Allocator.h"

namespace GameMemorySystem
{

// Type used for storing metadata in the header/footer of memory block
// Rightmost bit contains isFree flag, all other bits contain size.
typedef size_t blockMetadata_t;

// Minimum size of a block in bytes
constexpr size_t minBlockSize = 2 * sizeof(blockMetadata_t) + 2 * sizeof(blockMetadata_t*);

// Allows for allocations and frees to occur in any order and for different sized objects
// Stores a list of allocated and free blocks.
class DynamicListAllocator : public Allocator
{
private:
	// Ptr to the start and end blocks of the list
	blockMetadata_t* m_pListStart = nullptr;
	blockMetadata_t* m_pListEnd = nullptr;

	// Special metadata values that indicate the start & end of the list
	static constexpr blockMetadata_t listStartMeta = sizeof(blockMetadata_t) << 1;
	static constexpr blockMetadata_t listEndMeta = (sizeof(blockMetadata_t) << 1) | 1;

public:
	// Bidirectional iterator for traversing blocks of memory
	class iterator
	{
	private:
		// Points to the header of the curent block
		blockMetadata_t* m_pBlockHeader;
	public:
		iterator() : m_pBlockHeader(nullptr) { }
		iterator(blockMetadata_t* pBlockPtr) : m_pBlockHeader(pBlockPtr) { }
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& other) { return m_pBlockHeader == other.m_pBlockHeader; }
		bool operator!=(const iterator& other) { return !(*this == other); }
		blockMetadata_t* operator*() { return m_pBlockHeader; }
		blockMetadata_t** operator->() { return &m_pBlockHeader; }
	};

private:
	// Splits given block into two blocks.
	// Size of first new block is given in bytes.
	// Both blocks are marked "free"
	void splitBlock(blockMetadata_t* pBlockHeader, size_t sizeOfFirstNewBlock);

	// Combines given block with the block after it.
	// The resulting block is marked "free"
	void combineWithNextBlock(blockMetadata_t* pBlockHeader);

public:
	// Make sure default constructor is added
	DynamicListAllocator() = default;

	// Delete copy & move constructors/assignment operators
	DynamicListAllocator(const DynamicListAllocator&) = delete;
	DynamicListAllocator& operator=(const DynamicListAllocator&) = delete;
	DynamicListAllocator(DynamicListAllocator&&) = delete;
	DynamicListAllocator& operator=(DynamicListAllocator&&) = delete;

	// Initializes allocator for use
	void init(U8* pMemLocation, size_t bytes);

	// Prints debug info
	// TODO: create const iterator and make this const
	void print();

	// Allocates new memory block that will fit size bytes with given alignment
	void* alloc(size_t size, Alignment align) override;

	// Frees the memory block that given ptr lies within
	void free(void* ptr) override;

	// Returns true if the given memory address is within the list
	bool containsAddress(void* ptr) const;

	// Writes metadata that defines the start and end of a memory block.
	// Returns ptr to the header of the block
	blockMetadata_t* writeBlock(U8* pStart, bool isFree, size_t size);

	// Returns iterator to the first block
	iterator begin();

	// Returns iterator to block after the last block
	iterator end();

	// Writes the given metadata into the given location
	static void writeMetadata(blockMetadata_t* pLocation, bool isFree, size_t size);

	// Gets footer metadata from header metadata
	static blockMetadata_t* getFooter(blockMetadata_t* pBlockHeader);

	// Returns true if the block is free
	static bool isFree(const blockMetadata_t* pMetadata);

	// Marks given block as free
	static void setFree(blockMetadata_t* pBlockHeader);

	// Marks given block as not free
	static void setNotFree(blockMetadata_t* pBlockHeader);

	// Returns the size of the block
	static size_t getSize(const blockMetadata_t* pMetadata);

	// Returns header of the previous block
	static blockMetadata_t* getPrevBlock(blockMetadata_t* pBlockHeader);

	// Returns header of the next block
	static blockMetadata_t* getNextBlock(blockMetadata_t* pBlockHeader);

	// Returns true if given block is the special "start" block
	static bool isStartOfList(const blockMetadata_t* pBlockHeader);

	// Returns true if given block is the special "end" block
	static bool isEndOfList(const blockMetadata_t* pBlockHeader);
};
}