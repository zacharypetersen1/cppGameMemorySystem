#include "MemoryGlobals.h"
#include <cstddef>
#include "MemorySystem.h"

GameMemorySystem::Allocator gAllocator = GameMemorySystem::Allocator::dynamic;
GameMemorySystem::MemorySystem gMemSystem;
int gDefaultAlign = 16;

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
	return gMemSystem.alloc(size, GameMemorySystem::Alignment(gDefaultAlign), gAllocator);
}

void* operator new[](size_t size)
{
	return gMemSystem.alloc(size, GameMemorySystem::Alignment(gDefaultAlign), gAllocator);
}