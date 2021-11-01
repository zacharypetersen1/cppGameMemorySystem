#include "MemoryGlobals.h"
#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cstdlib>
#include <iostream>

class foo
{
	int beef = 0xbeefbeef;
};

int main()
{
	gMemSystem.startup(2048, 32, 32);
	gAllocator = GameMemorySystem::Allocator::dynamic;
	foo* pFoo = new foo();

	return 0;
}