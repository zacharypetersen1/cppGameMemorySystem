#include "MemorySystem.h"
#include <assert.h>
#include <stdlib.h>

namespace GameMemorySystem
{
void MemorySystem::startup()
{
	// Size of memory region granted to each allocator in bytes
	const size_t dynamicBytes = 2048;
	const size_t persistantBytes = 32;
	const size_t oneFrameBytes = 32;
	const size_t pool16Bytes = 16 * 5;
	const size_t pool32Bytes = 32 * 5;

	// Allocate memory region for all allocators
	size_t m_totalBytes = dynamicBytes + persistantBytes + oneFrameBytes + pool16Bytes + pool32Bytes;
	m_pMemBlock = malloc(m_totalBytes);

	// Init all allocators
	// This defines the memory layout - all allocators are placed one
	// after another within the allocated memory region.
	U8* pBytePtr = static_cast<U8*>(m_pMemBlock);
	m_dynamicAlloc.init(pBytePtr, dynamicBytes);
	pBytePtr += dynamicBytes;
	m_persistantAlloc.init(pBytePtr, persistantBytes);
	pBytePtr += persistantBytes;
	m_singleFrameAlloc.init(pBytePtr, oneFrameBytes);
	pBytePtr += oneFrameBytes;
	m_poolAlloc16.init(pBytePtr, 16, pool16Bytes / 16);
	pBytePtr += pool16Bytes;
	m_poolAlloc32.init(pBytePtr, 32, pool32Bytes / 32);
}

void MemorySystem::shutdown()
{
	free(m_pMemBlock);
}

}