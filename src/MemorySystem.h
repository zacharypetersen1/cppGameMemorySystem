#pragma once
#include "DynamicListAllocator.h"
#include "StackAllocator.h"
#include "MemoryUtils.h"
#include "PoolAllocator.h"

namespace GameMemorySystem
{
// Maintains the memory block and allocators
class MemorySystem
{
private:
	// Ptr to the start of the memory block
	void* m_pMemBlock = nullptr;

	// Size of the entire memory block used by all allocators in bytes
	size_t m_totalBytes = 0;
	
public:
	// Used to allocate to dynamic memory 
	DynamicListAllocator m_dynamicAlloc;

	// Used to allocate to memory that is valid for the rest of the frame
	StackAllocator m_singleFrameAlloc;

	// Used to allocate memory that will stay valid while the process is running
	StackAllocator m_persistantAlloc;

	// 16 byte pool alocator
	PoolAllocator m_poolAlloc16;

	// 32 byte pool alocator
	PoolAllocator m_poolAlloc32;

public:
	// Make sure default constructor is added because we are deleting other constructors below.
	MemorySystem() = default;

	// Delete copy & move constructors/assignment operators
	MemorySystem(const MemorySystem&) = delete;
	MemorySystem& operator=(const MemorySystem&) = delete;
	MemorySystem(MemorySystem&&) = delete;
	MemorySystem& operator=(MemorySystem&&) = delete;

	// Allocates the memory block and sets up the memory system for use.
	void startup();

	// Releases memory & any other resources held by the memory system and allocators
	void shutdown();

	// Clears memory used by the single frame allocator
	void clearSingleFrameMemory() { m_singleFrameAlloc.clear(); }
};
}