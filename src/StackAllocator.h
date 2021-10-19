#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{
class StackAllocator
{
private:
	// Ptr to the beginning of memory block
	void* pMem = nullptr;

	// Size of memory block in bytes
	size_t memSize = 0;

	// Root location of the memory block
	uintptr_t root = 0;

	// Top location of stack on the memory block
	uintptr_t top = 0;

public:
	StackAllocator() = default;
	// Don't allow copying
	StackAllocator(const StackAllocator&) = delete;
	StackAllocator& operator=(const StackAllocator&) = delete;

	// Creates the memory block
	void init(size_t bytes);

	// Destroys the memory block
	void destroy();

	// Clears the stack
	void clear();

	// Prints debug info about memory block
	void print();

	// Allocates object of given size with given alignment
	void* alloc(size_t size, Alignment align);
};
}
