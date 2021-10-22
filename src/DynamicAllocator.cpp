#include "DynamicAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

namespace GameMemorySystem
{
void DynamicAllocator::init(void* pMemStart, size_t bytes)
{
	m_pMem = pMemStart;
	m_size = bytes;
}

void DynamicAllocator::print() const
{
	std::cout << "MemHeap size: " << std::dec << m_size << std::endl;
	for (size_t i = 0; i < m_size; ++i)
	{
		std::cout << std::hex << (size_t)m_pMem + i << std::dec << ":\n";
	}
}

void* DynamicAllocator::alloc(size_t size, Alignment align)
{
	return malloc(size);
}
}