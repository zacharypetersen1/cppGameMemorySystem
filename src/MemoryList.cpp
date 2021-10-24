#include "MemoryList.h"

namespace GameMemorySystem
{

void MemoryBlock::init(bool isFree, size_t size)
{
	// Combine isFree and size into header
	size_t isFree_AsSizeT = static_cast<size_t>(isFree);
	size_t shiftedSize = size << 1;
	m_header = isFree_AsSizeT | shiftedSize;
}

}