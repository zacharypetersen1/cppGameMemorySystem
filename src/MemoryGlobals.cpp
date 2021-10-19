#include "MemoryGlobals.h"
#include "MemorySystem.h"

GameMemorySystem::Allocator gAllocator = GameMemorySystem::Allocator::dynamic;
GameMemorySystem::MemorySystem gMemSystem;

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
	return gMemSystem.alloc(size, GameMemorySystem::Alignment(16), gAllocator);
}

void* operator new[](size_t size)
{
	return gMemSystem.alloc(size, GameMemorySystem::Alignment(16), gAllocator);
}