#include "MemorySystem.h"
#include <assert.h>
#include <stdlib.h>

namespace GameMemorySystem
{
void MemorySystem::startup(size_t dynamicMemBytes, size_t persistantMemBytes, size_t oneFrameMemBytes)
{
	size_t totalBytes = dynamicMemBytes + persistantMemBytes + oneFrameMemBytes;
	pMemBlock = malloc(totalBytes);

	// Use char ptr to figure out where each allocator's memory region should start
	char* pBytePtr = static_cast<char*>(pMemBlock);
	dynamicAlloc.init(pBytePtr, dynamicMemBytes);
	pBytePtr += dynamicMemBytes;
	persistantAlloc.init(pBytePtr, persistantMemBytes);
	pBytePtr += persistantMemBytes;
	oneFrameAlloc.init(pBytePtr, oneFrameMemBytes);
}

void MemorySystem::shutdown()
{
	free(pMemBlock);
}

void MemorySystem::print()
{
	dynamicAlloc.print();
	persistantAlloc.print();
	oneFrameAlloc.print();
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