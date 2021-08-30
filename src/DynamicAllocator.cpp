#include "DynamicAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

void DynamicAllocator::init(size_t bytes)
{
	assert(bytes > 0);
	size = bytes;
	pMem = malloc(bytes);
}

void DynamicAllocator::destroy()
{
	free(pMem);
}

void DynamicAllocator::print()
{
	std::cout << "MemHeap size: " << std::dec << size << std::endl;
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << std::hex << (size_t)pMem + i << std::dec << ":\n";
	}
}

void* DynamicAllocator::alloc(size_t size, Alignment align)
{
	return malloc(size);
}