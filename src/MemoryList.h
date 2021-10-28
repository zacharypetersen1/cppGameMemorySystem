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
constexpr size_t minBlockSize = 2 * sizeof(metadata_type) + 2 * sizeof(blockPtr);

// Maintains memory blocks in a list structure
class MemoryList
{
private:
	// Ptr to the start of the list
	blockPtr m_pListStart = nullptr;
	blockPtr m_pListEnd = nullptr;

	// Special metadata values that indicate blocks lying at the start/end of list
	static constexpr metadata_type listStartMeta = sizeof(metadata_type) << 1;
	static constexpr metadata_type listEndMeta = (sizeof(metadata_type) << 1) | 1;

private:
	// Writes the given metadata into the given location
	void writeMetadata(metadata_type* pLocation, bool isFree, size_t size);

public:
	class iterator
	{
	public:
		blockPtr m_pBlockPtr;
		
		iterator() : m_pBlockPtr(nullptr) { }
		iterator(blockPtr pBlockPtr) : m_pBlockPtr(pBlockPtr) { }

		iterator& operator++()
		{
			m_pBlockPtr = getNextBlock(m_pBlockPtr);
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++*this;
			return temp;
		}

		iterator& operator--()
		{
			m_pBlockPtr = getPrevBlock(m_pBlockPtr);
			return *this;
		}

		iterator operator--(int)
		{
			iterator temp = *this;
			--*this;
			return temp;
		}

		bool operator==(const iterator& other)
		{
			return m_pBlockPtr == other.m_pBlockPtr;
		}

		bool operator!=(const iterator& other)
		{
			return !(*this == other);
		}

		blockPtr operator*()
		{
			return m_pBlockPtr;
		}

		blockPtr* operator->()
		{
			return &m_pBlockPtr;
		}
	};

	// Writes metadata that defines the start and end of a memory block.
	// Returns ptr to the header of the block
	blockPtr createMemoryBlock(U8* pStart, bool isFree, size_t size);

	// Initializes the memory list
	void init(U8* pMemStart, size_t size);

	// Returns begining of list
	iterator begin();

	// Returns end of the list
	iterator end();

	// Returns true if the block is free
	static bool isFree(const blockPtr pMetadata);

	// Returns the size of the block
	static size_t getSize(const blockPtr pMetadata);

	// Returns header of the next block
	static blockPtr getPrevBlock(blockPtr pBlock);

	// Returns header of the previous block
	static blockPtr getNextBlock(blockPtr pBlock);

	// Returns true if given block is the first block
	static bool isStartOfList(const blockPtr bBlock);

	// Returns true if given block is the last block
	static bool isEndOfList(const blockPtr bBlock);
};

}
