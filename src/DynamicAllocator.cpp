#include "DynamicAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

namespace GameMemorySystem
{
void DynamicAllocator::init(void* pMemStart, size_t bytes)
{
	pMem = pMemStart;
	size = bytes;
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
}