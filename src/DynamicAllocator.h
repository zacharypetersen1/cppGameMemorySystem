#pragma once
#include "Enums.h"

class DynamicAllocator
{
private:
	void* pMem;
	size_t size;
public:
	void init(size_t size);
	void destroy();
	void print();
	void* alloc(size_t size, Alignment align);
};

