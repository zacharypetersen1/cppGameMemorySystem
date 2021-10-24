#pragma once
#include "MemoryUtils.h"
#include "MemoryHeap.h"

namespace GameMemorySystem
{

// Contains metadata for a memory block
struct BlockHeader
{
private:
	// Rightmost bit contains isFree flag.
	// Leftmost seven bits contain alignment offset.
	U8 m_isFreeAndAlignOffset = 0;

	// Size of block in bytes
	size_t m_size = 0;

public:
	// Initializes member data
	void init(bool isFree, U8 alignOffset, size_t size);

	// Returns true if this block is free
	inline bool isFree() const { return (m_isFreeAndAlignOffset & 1) > 0; }
	
	// Returns the alignment offset of this block
	inline U8 getAlignmentOffset() const { return m_isFreeAndAlignOffset >> 1; }

	// Returns the block size
	inline size_t getSize() const { return m_size; }
};

// Allows for allocations and frees to occur in any order
class DynamicAllocator
{
private:
	// Location in memory where this allocator starts
	U8* m_pMem;

	// Size in bytes of this allocator's memory region
	size_t m_size;
public:
	// Make sure default constructor is added because we are deleting other constructors below.
	DynamicAllocator() = default;

	// Delete copy & move constructors/assignment operators
	DynamicAllocator(const DynamicAllocator&) = delete;
	DynamicAllocator& operator=(const DynamicAllocator&) = delete;
	DynamicAllocator(DynamicAllocator&&) = delete;
	DynamicAllocator& operator=(DynamicAllocator&&) = delete;

	// Initializes allocator for use
	void init(U8* pMemLocation, size_t bytes);

	// Prints debug info about memory block
	void print() const;

	// Allocates object of given size with given alignment
	void* alloc(size_t size, Alignment align);
};
}