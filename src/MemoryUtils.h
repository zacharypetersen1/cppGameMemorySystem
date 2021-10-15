#pragma once
#include "Types.h"
#include <assert.h>

class Alignment
{
public:
	explicit Alignment(int a);
	inline int getValue() { return value; }
private:
	int value;
};

enum class Allocator { dynamic, oneFrame, persistant };
uintptr_t Align(uintptr_t addr, Alignment align);