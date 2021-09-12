#include "StackAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

void StackAllocator::init(size_t bytes)
{
	assert(bytes > 0);
	memSize = bytes;
	pMem = malloc(bytes);
	root = reinterpret_cast<uintptr_t>(pMem);
	top = root;
}

void StackAllocator::destroy()
{
	free(pMem);
}

void StackAllocator::clear()
{
	top = root;
}

void StackAllocator::print()
{
	std::cout << std::dec << "MemStack size: " << memSize << std::endl;
	for (size_t i = 0; i < memSize; ++i)
	{
		std::cout << std::hex << (size_t)pMem + i << std::dec << ":\n";
	}
}

void* StackAllocator::alloc(size_t size, U8 align)
{
	// Get aligned position on stack for this alloc
	uintptr_t ptr = Align(top, align);
	
	// Check if this alloc will excede size of stack
	if (ptr + size > root + memSize)
	{
		throw std::bad_alloc();
	}

	top = ptr + size;
	return reinterpret_cast<void*>(ptr);
}