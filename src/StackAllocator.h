#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{
class StackAllocator
{
private:
	// Ptr to the beginning of stack
	void* pMem = nullptr;

	// Size of stack in bytes
	size_t stackSize = 0;

	// Root location of the stack
	uintptr_t root = 0;

	// Top location of stack
	uintptr_t top = 0;

public:
	StackAllocator() = default;
	// Don't allow copying
	StackAllocator(const StackAllocator&) = delete;
	StackAllocator& operator=(const StackAllocator&) = delete;

	// Creates the memory block
	void init(void* memStart, size_t bytes);

	// Clears the stack
	void clear();

	// Prints debug info about memory block
	void print();

	// Allocates object of given size with given alignment
	void* alloc(size_t size, Alignment align);
};
}
