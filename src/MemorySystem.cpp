#include "MemorySystem.h"
#include <assert.h>

namespace GameMemorySystem
{
void MemorySystem::startup()
{
	dynamicAlloc.init(32);
	oneFrameAlloc.init(32);
	persistantAlloc.init(32);
}

void MemorySystem::shutdown()
{
	dynamicAlloc.destroy();
	oneFrameAlloc.destroy();
	persistantAlloc.destroy();
}

void MemorySystem::print()
{
	dynamicAlloc.print();
	oneFrameAlloc.print();
	persistantAlloc.print();
}

void* MemorySystem::alloc(size_t size, Alignment align, Allocator allocType)
{
	// TODO Assert that alignment is power of 2

	// Select allocator
	switch (allocType)
	{
	case Allocator::dynamic:
		return dynamicAlloc.alloc(size, align);
	case Allocator::oneFrame:
		return oneFrameAlloc.alloc(size, align);
	case Allocator::persistant:
		return dynamicAlloc.alloc(size, align);
	default:
		assert(0 == 1, "No alloc function for give alloc type was defined.");
		return nullptr;
	}
}
}