#include "MemorySystem.h"
#include <assert.h>
#include <stdlib.h>

namespace GameMemorySystem
{
void MemorySystem::startup(size_t dynamicBytes, size_t persistantBytes, size_t oneFrameBytes)
{
	size_t m_totalBytes = dynamicBytes + persistantBytes + oneFrameBytes;
	m_pMemBlock = malloc(m_totalBytes);

	// Use char ptr to figure out where each allocator's memory region should start
	char* pBytePtr = static_cast<char*>(m_pMemBlock);
	m_dynamicAlloc.init(pBytePtr, dynamicBytes);
	pBytePtr += dynamicBytes;
	m_persistantAlloc.init(pBytePtr, persistantBytes);
	pBytePtr += persistantBytes;
	m_singleFrameAlloc.init(pBytePtr, oneFrameBytes);
}

void MemorySystem::shutdown()
{
	free(m_pMemBlock);
}

void MemorySystem::print() const
{
	m_dynamicAlloc.print();
	m_persistantAlloc.print();
	m_singleFrameAlloc.print();
}

void* MemorySystem::alloc(size_t size, Alignment align, Allocator allocType)
{
	// Select allocator
	switch (allocType)
	{
	case Allocator::dynamic:
		return m_dynamicAlloc.alloc(size, align);
	case Allocator::singleFrame:
		return m_singleFrameAlloc.alloc(size, align);
	case Allocator::persistant:
		return m_dynamicAlloc.alloc(size, align);
	default:
		assert(0 == 1, "No alloc function for give alloc type was defined.");
		return nullptr;
	}
}
}