#include "Allocator.h"

namespace GameMemorySystem
{

bool Allocator::containsAddress(void* ptr)
{
	return ptr >= m_pMem && ptr < (m_pMem + m_size);
}

}