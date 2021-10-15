#include "MemoryGlobals.h"
#include <stdlib.h>
#include <iostream>

class bar
{
	U8 foo;
};

class foo
{
private:
	U32 bar;
	U32 baz;
};

int main()
{
	gMemSystem.startup();
	gMemSystem.print();
	foo* pFoo = new(Alignment(16), Allocator::oneFrame) foo();
	std::cout << "pFoo:" << std::hex << pFoo << std::endl;
	foo* pFoo2 = new(Alignment(16), Allocator::oneFrame) foo();
	std::cout << "pFoo2:" << std::hex << pFoo2 << std::endl;

	gMemSystem.clearFrameMemory();

	foo* pFoo3 = new(4, Allocator::oneFrame) foo();
	std::cout << "pFoo3:" << std::hex << pFoo3 << std::endl;
	foo* pFoo4 = new(4, Allocator::oneFrame) foo();
	std::cout << "pFoo4:" << std::hex << pFoo4 << std::endl;

	gMemSystem.shutdown();
	return 0;
}