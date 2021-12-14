#pragma once
#include "MemoryUtils.h"
#include "Allocator.h"

namespace GameMemorySystem
{

class PoolAllocator : public Allocator
{
private:
	struct Node {
		Node* next;
	};
	Node* m_pFreeNodesHead = nullptr;
	size_t m_elementSize = 0;
	size_t m_numElements = 0;

public:
	PoolAllocator() = default;
	void init(U8* m_pMemStart, size_t elementSize, size_t numElements);	
	void* alloc(size_t size, Alignment align) override;
	void free(void* ptr) override;
	
	// Prints all of the free nodes
	void printFreeNodes();
};

}