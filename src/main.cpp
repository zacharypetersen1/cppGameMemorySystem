#include "MemoryGlobals.h"

class foo
{
	int bar = 0xbeefbeef;
	float baz = 0.0f;
};

int main()
{
	// Call before making any allocations
	gMemSystem.startup();

	// Specify alignment & allocator when using new
	foo* pFoo1 = new(ALIGN(4), gMemSystem.m_persistantAlloc) foo();

	// Default allocator & alignment are used if not specified
	foo* pFoo2 = new foo(); // dynamic aloc & 16 byte aligned
	gDefaultAlignment = ALIGN(4);
	gDefaultAllocator = gMemSystem.m_persistantAlloc;
	foo* pFoo3 = new foo(); // Persistant aloc & 4 byte aligned

	// Omit allocator or alignment to use the defaults
	foo* pFoo4 = new(gMemSystem.m_singleFrameAlloc) foo();
	foo* pFoo5 = new(ALIGN(8)) foo();

	// Pool allocators are always aligned to size of pool elements
	foo* pFoo6 = new(gMemSystem.m_poolAlloc16) foo(); // 16 byte aligned
	foo* pFoo7 = new(gMemSystem.m_poolAlloc32) foo(); // 32 byte aligned

}