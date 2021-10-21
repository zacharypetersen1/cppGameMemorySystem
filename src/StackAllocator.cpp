#include "StackAllocator.h"
#include "MemoryUtils.h"
#include <stdlib.h>
#include <assert.h>
#include <iostream>

namespace GameMemorySystem
{
void StackAllocator::init(void* pMemStart, size_t bytes)
{
	pMem = pMemStart;
	stackSize = bytes;
	root = reinterpret_cast<uintptr_t>(pMem);
	top = root;
}

void StackAllocator::clear()
{
	top = root;
}

void StackAllocator::print()
{
	std::cout << std::dec << "MemStack size: " << stackSize << std::endl;
	for (size_t i = 0; i < stackSize; ++i)
	{
		std::cout << std::hex << (size_t)pMem + i << std::dec << ":\n";
	}
}

void* StackAllocator::alloc(size_t size, Alignment align)
{
	// Get aligned position on stack for this alloc
	uintptr_t ptr = Align(top, align);

	// Check if this alloc will excede size of stack
	if (ptr + size > root + stackSize)
	{
		throw std::bad_alloc();
	}

	top = ptr + size;
	return reinterpret_cast<void*>(ptr);
}
}