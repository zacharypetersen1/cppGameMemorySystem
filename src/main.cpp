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

	// Global operator new overloads:
	foo* pFoo1 = new foo();
	foo* pFoo2 = new(ALIGN(4)) foo();
	foo* pFoo3 = new(gMemSystem.m_poolAlloc16) foo();
	foo* pFoo4 = new(ALIGN(4), gMemSystem.m_persistantAlloc) foo();

	// If not specified, the default allocator & alignment are used
	//		initial values:
	//			gDefaultAllocator = gMemSystem.m_dynamicAlloc
	//			gDefaultAlignment = ALIGN(16)
	gDefaultAllocator = gMemSystem.m_singleFrameAlloc;
	gDefaultAlignment = ALIGN(8);

	// Pool allocators are always aligned to size of pool elements
	foo* pFoo5 = new(ALIGN(4), gMemSystem.m_poolAlloc16) foo(); // 16 byte aligned

	// Freeing the single frame memory:
	while (true)
	{
		// Start of frame
		foo* pFoo6 = new(ALIGN(4), gMemSystem.m_singleFrameAlloc) foo();
		
		// ... do stuff with pFoo6

		// End of frame
		gMemSystem.clearSingleFrameMemory();
		// All allocations made with gMemSystem.m_singleFrameAlloc are now freed
	}

	// Will free all dynamic memory resources
	gMemSystem.shutdown();
}