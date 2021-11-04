# Game Memory System #

A memory system for building a games in C++

### Why use this? ###
This memory system avoids making calls to the OS when using new/delete. It works by preallocating a large chunk of memory and manually managing dynamic allocations/deallocations. It provides these allocators to the programmer:

~               | Object Size | Deallocation time                   | Fragmentation | Implementation
--------------  |-------------|-------------------------------------|---------------|-----------------
**Persistent**  | Any         | When memory system is shut down.    | No            | Stack
**Single Frame**| Any         | At the end of the frame.            | No            | Stack
**Dynamic**     | Any         | When manually deleted.              | Yes           | List
