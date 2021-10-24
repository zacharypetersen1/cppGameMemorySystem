#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{
// Simple allocator where memory is freed in FIFO order
class StackAllocator
{
private:
	// Ptr to the beginning of stack
	U8* m_pMem = nullptr;

	// Size of stack in bytes
	size_t m_size = 0;

	// Root location of the stack
	uintptr_t m_root = 0;

	// Top location of stack
	uintptr_t m_top = 0;

public:
	// Make sure default constructor is added.
	StackAllocator() = default;

	// Delete copy & move constructors/assignment operators
	StackAllocator(const StackAllocator&) = delete;
	StackAllocator& operator=(const StackAllocator&) = delete;
	StackAllocator(StackAllocator&&) = delete;
	StackAllocator& operator=(StackAllocator&&) = delete;

	// Initializes stack for use
	void init(U8* memStart, size_t bytes);

	// Clears the stack
	void clear();

	// Prints debug info about memory block
	void print() const;

	// Allocates object of given size with given alignment
	void* alloc(size_t size, Alignment align);
};
}
