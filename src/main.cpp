#include "MemoryGlobals.h"
#include "MemoryUtils.h"
#include <cstdlib>
#include <iostream>

class foo
{
	int beef = 0xbeefbeef;
};

class bar
{
	long long int longbeef = 0xbeeeeeeeeeeeeeef;
};

int main()
{
	gMemSystem.startup(2048, 32, 32);
	gActiveAllocator = GameMemorySystem::Allocator::dynamic;
	foo* pFoo = new foo();
	bar* pBar = new bar();
	foo* pFoo2 = new foo();

	return 0;
}