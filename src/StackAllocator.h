#pragma once
#include "Enums.h"

class StackAllocator
{
private:
	void* pMem;
	size_t size;
public:
	void init(size_t bytes);
	void destroy();
	void print();
	void* alloc(size_t size, Alignment align);
};

