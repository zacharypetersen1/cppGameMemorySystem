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
	gMemSystem.print();
	foo* pFoo = new foo();
	gMemSystem.print();
	bar* pBar = new bar();
	gMemSystem.print();
	foo* pFoo2 = new foo();
	gMemSystem.print();
	delete pBar;
	gMemSystem.print();
	delete pFoo;
	gMemSystem.print();
	delete pFoo2;
	gMemSystem.print();
	return 0;
}