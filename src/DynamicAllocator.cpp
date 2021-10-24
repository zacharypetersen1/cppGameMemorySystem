#include "DynamicAllocator.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "MemoryUtils.h"

namespace GameMemorySystem
{
void BlockHeader::init(bool isFree, U8 alignOffset, size_t size)
{
	// Combine isFree and alignOffset
	U8 isFreeU8 = static_cast<U8>(isFree);
	U8 shiftedOffset = alignOffset << 1;
	m_isFreeAndAlignOffset = isFreeU8 | shiftedOffset;

	// Store size
	m_size = size;
}

void DynamicAllocator::init(U8* pMemStart, size_t bytes)
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