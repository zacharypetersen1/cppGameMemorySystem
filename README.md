# Game Memory System #

A memory system for building a games in C++. Inspired by chapter 6.2 on Memory Management in Jason Gregory's book *Game Engine Architecture* and by Christian Gyrling's post [Are we out of memory?](http://www.swedishcoding.com/2008/08/31/are-we-out-of-memory/).

### Why use this? ###
This memory system avoids making calls to the OS when using new/delete. It works by preallocating a large chunk of memory and manually managing dynamic allocations/deallocations. This may provide a speedup since the memory manager does not have to context switch to kernel mode to allocate memory. Also, this memory system provides different allocators aimed at specific use cases. Some of these allocators require less logic than a general purpose allocator, which may also speed up memory allocation. Lastly, this memory system centralizes all of the memory used by your game into a single chunk, which is partitioned into smaller chunks for the different allocators. You can configure the amount of memory distributed to each allocator as you see fit, and you will know where to look when writing tools to measure memory usage and debug memory problems.

### Allocators ###
Allocator                         | Object Size      | Deallocation time                   | Fragmentation | Implementation
----------------------------------|------------------|-------------------------------------|---------------|-----------------
**gMemSystem.m_persistantAlloc**  | Any              | When memory system is shut down.    | No            | Stack
**gMemSystem.m_singleFrameAlloc** | Any              | At the end of the frame.            | No            | Stack
**gMemSystem.m_dynamicAlloc**     | Any              | When manually deleted.              | Yes           | List
**gMemSystem.m_poolAlloc16**      | ≤ 16 bytes       | When manually deleted.              | No            | Pool
**gMemSystem.m_poolAlloc32**      | ≤ 32 bytes       | When manually deleted.              | No            | Pool

### Usage ###
```c++
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
```
