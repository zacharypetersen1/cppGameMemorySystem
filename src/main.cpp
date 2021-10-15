#include "MemoryGlobals.h"
#include "Types.h"
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
	// gMemorySystem provides access to global memory system.
	// Startup will allocate the memory block that it uses.
	gMemSystem.startup();
	gMemSystem.print();

	// Set gAllocator to control what type of memory the global
	// new operator will allocate to.
	gAllocator = Allocator::oneFrame;
	foo* pFoo = new foo();
	std::cout << "pFoo:" << std::hex << pFoo << std::endl;
	foo* pFoo2 = new foo();
	std::cout << "pFoo2:" << std::hex << pFoo2 << std::endl;

	// Call clearFrameMemory after the frame is done to clear
	// single frame memory region.
	gMemSystem.clearFrameMemory();

	foo* pFoo3 = new foo();
	std::cout << "pFoo3:" << std::hex << pFoo3 << std::endl;
	foo* pFoo4 = new foo();
	std::cout << "pFoo4:" << std::hex << pFoo4 << std::endl;

	gMemSystem.shutdown();
	return 0;
}