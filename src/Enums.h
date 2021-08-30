#pragma once
#include "Types.h"

enum class Allocator { dynamic, oneFrame, persistant };
class Alignment 
{ 
private:
	U8 value;
public:
	Alignment(U8 byteAlign): value(byteAlign) {}
	U8 getValue() { return value; }
};