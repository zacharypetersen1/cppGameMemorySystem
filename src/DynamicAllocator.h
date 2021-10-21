#pragma once
#include "MemoryUtils.h"
namespace GameMemorySystem
{
// Allows for allocations and frees to occur in any order
class DynamicAllocator
{
private:
	// Location in memory where this allocator starts
	void* m_pMem;

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
	void init(void* pMemLocation, size_t bytes);

	// Prints debug info about memory block
	void print();

	// Allocates object of given size with given alignment
	void* alloc(size_t size, Alignment align);
};
}