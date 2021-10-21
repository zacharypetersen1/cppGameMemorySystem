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
	// Initializes allocator for use
	void init(void* pMemLocation, size_t bytes);

	// Prints debug info about memory block
	void print();

	// Allocates object of given size with given alignment
	void* alloc(size_t size, Alignment align);
};
}