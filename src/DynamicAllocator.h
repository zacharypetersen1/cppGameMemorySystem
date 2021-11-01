#pragma once
#include "MemoryUtils.h"
#include "MemoryList.h"

namespace GameMemorySystem
{

// Allows for allocations and frees to occur in any order
class DynamicAllocator
{
private:
	// Location in memory where this allocator starts
	U8* m_pMem;

	// Size in bytes of this allocator's memory region
	size_t m_size;

	// Structure that manages memory
	MemoryList m_memList;

public:
	// Make sure default constructor is added
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