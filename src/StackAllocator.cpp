#include "StackAllocator.h"
#include "MemoryUtils.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

namespace GameMemorySystem
{
void StackAllocator::init(U8* pMemStart, size_t bytes)
{
	m_pMem = pMemStart;
	m_size = bytes;
	m_root = m_pMem;
	m_top = m_root;
}

void StackAllocator::clear()
{
	m_top = m_root;
}

void StackAllocator::print() const
{
	std::cout << std::dec << "MemStack size: " << m_size << std::endl;
	for (size_t i = 0; i < m_size; ++i)
	{
		std::cout << std::hex << (size_t)m_pMem + i << std::dec << ":\n";
	}
}

void* StackAllocator::alloc(size_t size, Alignment align)
{
	// Get aligned position on stack for this alloc
	U8* ptr = Align(m_top, align);

	// Check if this alloc will excede m_size of stack
	if (ptr + size > m_root + m_size)
	{
		throw std::bad_alloc();
	}

	m_top = ptr + size;
	return ptr;
}
}