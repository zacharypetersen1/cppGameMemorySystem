#pragma once
#include "MemoryUtils.h"

namespace GameMemorySystem
{

class Allocator
{
protected:
	// Ptr to the beginning of allocator's memory region
	U8* m_pMem = nullptr;

	// Size of allocator's memory region in bytes
	size_t m_size = 0;

public:
	virtual void* alloc(size_t size, Alignment align) = 0;
	bool containsAddress(void* ptr);
	virtual void free(void* ptr) {}
};

}