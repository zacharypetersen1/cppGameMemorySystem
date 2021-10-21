#pragma once
#include "DynamicAllocator.h"
#include "StackAllocator.h"
#include "MemoryUtils.h"

namespace GameMemorySystem
{
class MemorySystem
{
private:
	void* pMemBlock;
	DynamicAllocator dynamicAlloc;
	StackAllocator oneFrameAlloc;
	StackAllocator persistantAlloc;

public:
	void startup(size_t dynamicMemBytes, size_t persistantMemBytes, size_t oneFrameMemBytes);
	void shutdown();
	void print();
	void* alloc(size_t size, Alignment align, Allocator allocType);
	void clearFrameMemory() { oneFrameAlloc.clear(); }
};
}