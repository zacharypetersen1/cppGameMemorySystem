#include "MemoryGlobals.h"
#include <cstddef>
#include "MemorySystem.h"

GameMemorySystem::MemorySystem gMemSystem;
GameMemorySystem::Alignment gDefaultAlignment = GameMemorySystem::Alignment(16);

// Impliment custom new operators
void* operator new(size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator& allocator)
{
	return allocator.alloc(size, align);
}

void* operator new[](size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator& allocator)
{
	return allocator.alloc(size, align);
}

// Default to the dynamic allocator when no allocator is specified
void* operator new(size_t size)
{
	return gMemSystem.m_dynamicAlloc.alloc(size, gDefaultAlignment);
}

void* operator new[](size_t m_size)
{
	return  gMemSystem.m_dynamicAlloc.alloc(m_size, gDefaultAlignment);
}

void operator delete(void* ptr)
{
	if (gMemSystem.m_dynamicAlloc.containsAddress(ptr))
	{
		gMemSystem.m_dynamicAlloc.free(ptr);
	}
	else if (gMemSystem.m_poolAlloc16.containsAddress(ptr))
	{
		gMemSystem.m_poolAlloc16.free(ptr);
	}
	else if (gMemSystem.m_poolAlloc32.containsAddress(ptr))
	{
		gMemSystem.m_poolAlloc32.free(ptr);
	}
}

void operator delete[](void* ptr)
{
	if (gMemSystem.m_dynamicAlloc.containsAddress(ptr))
	{
		gMemSystem.m_dynamicAlloc.free(ptr);
	}
	else if (gMemSystem.m_poolAlloc16.containsAddress(ptr))
	{
		gMemSystem.m_poolAlloc16.free(ptr);
	}
	else if (gMemSystem.m_poolAlloc32.containsAddress(ptr))
	{
		gMemSystem.m_poolAlloc32.free(ptr);
	}
}