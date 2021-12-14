#include "PoolAllocator.h"
#include <new>
#include <iostream>

namespace GameMemorySystem
{

void PoolAllocator::init(U8* pMemStart, size_t elementSizeBytes, size_t numElements)
{
	// Each element must be large enough to hold pointer to next node for bookeeping
	assert(elementSizeBytes >= sizeof(Node*));

	m_pMem = pMemStart;
	m_size = elementSizeBytes * numElements;
	m_elementSize = elementSizeBytes;
	m_numElements = numElements;
	m_pFreeNodesHead = reinterpret_cast<Node*>(pMemStart);

	// Construct list of free nodes by pointing every node to the following node
	// Remember that the size of Node* and size of each elment in pool can be different 
	U8* iter = m_pMem;
	for (size_t i = 0; i < m_numElements - 1; ++i)
	{
		Node* node = reinterpret_cast<Node*>(iter);
		iter += elementSizeBytes;
		node->next = reinterpret_cast<Node*>(iter);
	}
	// Point the last node to nullptr
	Node* node = reinterpret_cast<Node*>(iter);
	node->next = nullptr;
}

void* PoolAllocator::alloc(size_t size, Alignment align)
{
	assert(size <= m_elementSize);

	// Pop off first free node and return it
	if (m_pFreeNodesHead == nullptr)
	{
		throw std::bad_alloc();
	}
	Node* result = m_pFreeNodesHead;
	m_pFreeNodesHead = m_pFreeNodesHead->next;
	return result;
}

void PoolAllocator::free(void* ptr)
{
	// Make sure ptr is within boundaries of mem chunk
	assert(containsAddress(ptr));

	// TODO make sure ptr aligns with a node, maybe with memStart % elemsize == ptr % elemSize?

	// Place into front of list of free nodes
	Node* pNode = reinterpret_cast<Node*>(ptr);
	pNode->next = m_pFreeNodesHead;
	m_pFreeNodesHead = pNode;
}

void PoolAllocator::printFreeNodes()
{
	Node* pNode = m_pFreeNodesHead;
	std::cout << "Free nodes:" << std::endl;
	while (pNode != nullptr)
	{
		std::cout << std::hex << pNode << std::endl;
		pNode = pNode->next;
	}
}

}