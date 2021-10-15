#pragma once
#include "MemorySystem.h"

extern Allocator gAllocator;
extern MemorySystem gMemSystem;
void* operator new(size_t size, Alignment align, Allocator allocType);
void* operator new[](size_t size, Alignment align, Allocator allocType);