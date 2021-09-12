#pragma once
#include "DynamicAllocator.h"
#include "StackAllocator.h"
#include "Types.h"
#include "Enums.h"

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
	void* alloc(size_t size, U8 align, Allocator allocType);
	void clearFrameMemory() { oneFrameAlloc.clear(); }
};
