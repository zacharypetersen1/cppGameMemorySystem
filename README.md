# Game Memory System #

A memory system for building a games in C++. Inspired by chapter 6.2 on Memory Management in Jason Gregory's book *Game Engine Architecture* and by [this post](http://www.swedishcoding.com/2008/08/31/are-we-out-of-memory/).

### Why use this? ###
This memory system avoids making calls to the OS when using new/delete. It works by preallocating a large chunk of memory and manually managing dynamic allocations/deallocations. This may provide a speedup since the memory manager does not have to context switch to kernel mode to allocate memory. Also, this memory system provides different allocators aimed at specific use cases. Some of these allocators require less logic than a general purpose allocator, which may also speed up memory allocation. Lastly, this memory system centralizes all of the memory used by your game into a single chunk, which is partitioned into smaller chunks for the different allocators. You can configure the amount of memory distributed to each allocator as you see fit, and you will know where to look when writing tools to measure memory usage and debug memory problems.

### Allocators ###
Type            | Object Size | Deallocation time                   | Fragmentation | Implementation
--------------  |-------------|-------------------------------------|---------------|-----------------
**Persistent**  | Any         | When memory system is shut down.    | No            | Stack
**Single Frame**| Any         | At the end of the frame.            | No            | Stack
**Dynamic**     | Any         | When manually deleted.              | Yes           | List
**Pool**        | Fixed Size  | When manually deleted.              | No            | List
