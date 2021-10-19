#pragma once
#include "MemorySystem.h"

extern GameMemorySystem::Allocator gAllocator;
extern GameMemorySystem::MemorySystem gMemSystem;
void* operator new(size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType);
void* operator new[](size_t size, GameMemorySystem::Alignment align, GameMemorySystem::Allocator allocType);