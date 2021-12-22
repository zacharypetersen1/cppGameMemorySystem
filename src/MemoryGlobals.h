#pragma once
#include "MemorySystem.h"
#include "Allocator.h"
#include "MemoryUtils.h"

// For reducing verbosity
#define ALIGN(bytes) GameMemorySystem::Alignment(bytes)

// The global memory system. This must be started up before it can be used.
extern GameMemorySystem::MemorySystem gMemSystem;

// The default alignment that will be used if none is specified.
extern GameMemorySystem::Alignment gDefaultAlignment;

// The default allocator
extern GameMemorySystem::Allocator& gDefaultAllocator;

// Varients of new operator
void* operator new(size_t size, GameMemorySystem::Alignment alignment, GameMemorySystem::Allocator& allocator);
void* operator new[](size_t size, GameMemorySystem::Alignment alignment, GameMemorySystem::Allocator& allocator);
void* operator new(size_t size, GameMemorySystem::Alignment alignment);
void* operator new[](size_t size, GameMemorySystem::Alignment alignment);
void* operator new(size_t size, GameMemorySystem::Allocator& allocator);
void* operator new[](size_t size, GameMemorySystem::Allocator& allocator);
