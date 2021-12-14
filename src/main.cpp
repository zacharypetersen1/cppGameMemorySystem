#include "MemoryGlobals.h"
#include "MemoryUtils.h"
#include <cstdlib>
#include <iostream>

class foo
{
	int beef = 0xbeefbeef;
};

class bar
{
	long long int longbeef = 0xbeeeeeeeeeeeeeef;
};

int main()
{
	gMemSystem.startup();
	gMemSystem.m_poolAlloc16.printFreeNodes();
	foo* f1 = new(GameMemorySystem::Alignment(16), gMemSystem.m_poolAlloc16) foo();
	gMemSystem.m_poolAlloc16.printFreeNodes();
	foo* f2 = new(GameMemorySystem::Alignment(16), gMemSystem.m_poolAlloc16) foo();
	gMemSystem.m_poolAlloc16.printFreeNodes();
	delete f1;
	gMemSystem.m_poolAlloc16.printFreeNodes();
	delete f2;
	gMemSystem.m_poolAlloc16.printFreeNodes();

	gMemSystem.m_dynamicAlloc.print();
	foo* df1 = new(GameMemorySystem::Alignment(sizeof(int)), gMemSystem.m_dynamicAlloc) foo();
	gMemSystem.m_dynamicAlloc.print();
	bar* db1 = new(GameMemorySystem::Alignment(16), gMemSystem.m_dynamicAlloc) bar();
	gMemSystem.m_dynamicAlloc.print();
	delete db1;
	gMemSystem.m_dynamicAlloc.print();
	delete df1;
	gMemSystem.m_dynamicAlloc.print();
}