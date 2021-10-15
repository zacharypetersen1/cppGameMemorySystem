#include "MemoryGlobals.h"
#include "MemorySystem.h"

Allocator gAllocator = Allocator::dynamic;
MemorySystem gMemSystem;

void* operator new(size_t size, Alignment align, Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

void* operator new[](size_t size, Alignment align, Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

void* operator new(size_t size)
{
	return gMemSystem.alloc(size, Alignment(16), gAllocator);
}

void* operator new[](size_t size)
{
	return gMemSystem.alloc(size, Alignment(16), gAllocator);
}