#include "MemoryUtils.h"

uintptr_t Align(uintptr_t addr, U8 align)
{
	assert(align > 0);
	U8 mask = align - 1;
	assert((align& mask) == 0); // Ensure power of 2
	return (addr + mask) & ~mask;
}