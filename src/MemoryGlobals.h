#pragma once
#include "MemorySystem.h"
#include "Allocator.h"
#include "MemoryUtils.h"

// The global memory system. This must be initialized before it can be used.
extern GameMemorySystem::MemorySystem gMemSystem;

// The default alignment that will be used if none is specified.
extern GameMemorySystem::Alignment gDefaultAlignment;

// Varient of new operator allowing alignment and Allocator to be specified.
void* operator new(size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator& allocator);
void* operator new[](size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator& allocator);
void operator delete(void* ptr, GameMemorySystem::Allocator& allocator);
void operator delete[](void* ptr, GameMemorySystem::Allocator& allocator);