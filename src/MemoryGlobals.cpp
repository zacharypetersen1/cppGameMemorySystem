#include "MemoryGlobals.h"
#include <cstddef>
#include "MemorySystem.h"

GameMemorySystem::Allocator gAllocator = GameMemorySystem::Allocator::dynamic;
GameMemorySystem::MemorySystem gMemSystem;
GameMemorySystem::Alignment gDefaultAlignment = GameMemorySystem::Alignment(16);

void* operator new(size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

void* operator new[](size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

void* operator new(size_t size)
{
	return gMemSystem.alloc(size, gDefaultAlignment, gAllocator);
}

void* operator new[](size_t m_size)
{
	return gMemSystem.alloc(m_size, gDefaultAlignment, gAllocator);
}