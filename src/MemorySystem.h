#pragma once
#include "DynamicAllocator.h"
#include "StackAllocator.h"
#include "MemoryUtils.h"

class MemorySystem
{
private:
	DynamicAllocator dynamicAlloc;
	StackAllocator oneFrameAlloc;
	StackAllocator persistantAlloc;

public:
	void startup();
	void shutdown();
	void print();
	void* alloc(size_t size, Alignment align, Allocator allocType);
	void clearFrameMemory() { oneFrameAlloc.clear(); }
};
