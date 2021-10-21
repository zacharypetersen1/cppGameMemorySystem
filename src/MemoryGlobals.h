#pragma once
#include "MemorySystem.h"

// The global memory system. This must be initialized before it can be used.
extern GameMemorySystem::MemorySystem gMemSystem;

// The allocator that will be used if none is specified.
// Use this to control where third party libraries allocate memory with new.
extern GameMemorySystem::Allocator gAllocator;

// The default alignment that will be used if none is specified.
extern GameMemorySystem::Alignment gDefaultAlignment;

// Varient of new operator allowing alignment and Allocator to be specified.
void* operator new(size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType);
void* operator new[](size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType);