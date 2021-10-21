#pragma once
#include "MemoryUtils.h"
namespace GameMemorySystem
{
class DynamicAllocator
{
private:
	void* pMem;
	size_t size;
public:
	void init(void* memLocation, size_t bytes);
	void print();
	void* alloc(size_t size, Alignment align);
};
}