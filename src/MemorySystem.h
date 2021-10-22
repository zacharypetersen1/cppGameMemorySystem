#pragma once
#include "DynamicAllocator.h"
#include "StackAllocator.h"
#include "MemoryUtils.h"

namespace GameMemorySystem
{
// Maintains the memory block and allocators
class MemorySystem
{
private:
	// Ptr to the start of the memory block
	void* m_pMemBlock;
	
	// Used to allocate to dynamic memory 
	DynamicAllocator m_dynamicAlloc;

	// Used to allocate to memory that is valid for the rest of the frame
	StackAllocator m_singleFrameAlloc;

	// Used to allocate memory that will stay valid while the process is running
	StackAllocator m_persistantAlloc;

	// Size of the entire memory block used by all allocators in bytes
	size_t m_totalBytes;

public:
	// Make sure default constructor is added because we are deleting other constructors below.
	MemorySystem() = default;

	// Delete copy & move constructors/assignment operators
	MemorySystem(const MemorySystem&) = delete;
	MemorySystem& operator=(const MemorySystem&) = delete;
	MemorySystem(MemorySystem&&) = delete;
	MemorySystem& operator=(MemorySystem&&) = delete;

	// Allocates the memory block and sets up the memory system for use.
	// Use the parameters to specify how many bytes each allocator should recieve.
	void startup(size_t dynamicMemBytes, size_t persistantMemBytes, size_t oneFrameMemBytes);

	// Releases memory & any other resources held by the memory system and allocators
	void shutdown();

	// Prints out debug information about the memory block
	void print() const;

	// Allocates memory using the specified allocator
	void* alloc(size_t m_size, Alignment align, Allocator allocType);

	// Clears memory used by the single frame allocator
	void clearSingleFrameMemory() { m_singleFrameAlloc.clear(); }
};
}