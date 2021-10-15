#include "MemoryGlobals.h"
#include "MemorySystem.h"

MemorySystem gMemSystem;

void* operator new(size_t size, Alignment align, Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

void* operator new[](size_t size, Alignment align, Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}