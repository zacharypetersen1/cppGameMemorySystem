#include "StackAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

void StackAllocator::init(size_t bytes)
{
	assert(bytes > 0);
	size = bytes;
	pMem = malloc(bytes);
}

void StackAllocator::destroy()
{
	free(pMem);
}

void StackAllocator::print()
{
	std::cout << std::dec << "MemStack size: " << size << std::endl;
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << std::hex << (size_t)pMem + i << std::dec << ":\n";
	}
}

void* StackAllocator::alloc(size_t size, Alignment align)
{
	return malloc(size);
}