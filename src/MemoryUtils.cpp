#include "MemoryUtils.h"

namespace GameMemorySystem
{
Alignment::Alignment(int a) : m_value(a)
{
	assert(a > 0);
	int check = a & (a - 1);
	assert(check == 0); // Power of 2
}

size_t getMaxPayloadSize(size_t size, Alignment align)
{
	return size + align.getValue();
}

uintptr_t Align(uintptr_t addr, Alignment align)
{
	int mask = align.getValue() - 1;
	return (addr + mask) & ~mask;
}
}