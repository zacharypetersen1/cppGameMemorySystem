#include "MemoryGlobals.h"
#include <cstddef>
#include "MemorySystem.h"

GameMemorySystem::MemorySystem gMemSystem;
GameMemorySystem::Alignment gDefaultAlignment = GameMemorySystem::Alignment(16);
GameMemorySystem::Allocator& gDefaultAllocator = gMemSystem.m_dynamicAlloc;

// Impliment new operators
void* operator new(size_t size, GameMemorySystem::Alignment alignment, GameMemorySystem::Allocator& allocator)
{
	return allocator.alloc(size, alignment);
}

void* operator new[](size_t size, GameMemorySystem::Alignment alignment, GameMemorySystem::Allocator& allocator)
{
	return allocator.alloc(size, alignment);
}

void* operator new(size_t size, GameMemorySystem::Alignment alignment)
{
	return gDefaultAllocator.alloc(size, alignment);
}

void* operator new[](size_t size, GameMemorySystem::Alignment alignment)
{
	return gDefaultAllocator.alloc(size, alignment);
}

void* operator new(size_t size, GameMemorySystem::Allocator& allocator)
{
	return allocator.alloc(size, gDefaultAlignment);
}

void* operator new[](size_t size, GameMemorySystem::Allocator& allocator)
{
	return allocator.alloc(size, gDefaultAlignment);
}

// Use default allocator & alignment if not specified
void* operator new(size_t size)
{
	return gDefaultAllocator.alloc(size, gDefaultAlignment);
}

void* operator new[](size_t size)
{
	return gDefaultAllocator.alloc(size, gDefaultAlignment);
}

// Impliment delete operators
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