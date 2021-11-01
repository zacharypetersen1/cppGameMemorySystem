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

U8* Align(U8* pAddr, Alignment align)
{
	intptr_t intPtrAddr = reinterpret_cast<intptr_t>(pAddr);
	int mask = align.getValue() - 1;
	intptr_t alignedAddr = (intPtrAddr + mask) & ~mask;
	return reinterpret_cast<U8*>(alignedAddr);
}

U8* AlignWithSpaceForPrefix(U8* pAddr, Alignment align)
{
	// Ensure there is at least 1 byte of space for prefix
	return Align(pAddr + 1, align);
}

}