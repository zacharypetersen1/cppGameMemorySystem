#include "MemoryGlobals.h"
#include <cstddef>
#include "MemorySystem.h"

GameMemorySystem::Allocator gActiveAllocator = GameMemorySystem::Allocator::dynamic;
GameMemorySystem::MemorySystem gMemSystem;
GameMemorySystem::Alignment gDefaultAlignment = GameMemorySystem::Alignment(16);

// Impliment custom new operators
void* operator new(size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

void* operator new[](size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

// Impliment global new & delete operators
void* operator new(size_t size)
{
	return gMemSystem.alloc(size, gDefaultAlignment, gActiveAllocator);
}

void* operator new[](size_t m_size)
{
	return gMemSystem.alloc(m_size, gDefaultAlignment, gActiveAllocator);
}

void operator delete(void* ptr)
{
	gMemSystem.free(ptr);
}

void operator delete[](void* ptr)
{
	gMemSystem.free(ptr);
}