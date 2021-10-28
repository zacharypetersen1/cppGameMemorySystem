#include "MemoryGlobals.h"
#include "MemoryList.h"
#include "MemoryUtils.h"
#include <cstdlib>
#include <iostream>

int main()
{
	GameMemorySystem::U8* pMem = (GameMemorySystem::U8*)malloc(2048);
	GameMemorySystem::MemoryList memList;
	memList.init(pMem, 2048);
	for (GameMemorySystem::blockPtr pBlock : memList)
	{
		std::cout << pBlock << "\n";
	}

	return 0;
}