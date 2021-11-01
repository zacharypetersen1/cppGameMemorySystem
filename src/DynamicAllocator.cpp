#include "DynamicAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "MemoryUtils.h"
#include "MemoryList.h"

namespace GameMemorySystem
{

void DynamicAllocator::init(U8* pMemStart, size_t bytes)
{
	m_pMem = pMemStart;
	m_size = bytes;
	m_memList.init(m_pMem, m_size);
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
	return m_memList.alloc(size, align);
}

}