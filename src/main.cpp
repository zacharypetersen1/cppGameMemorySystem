#include "MemorySystem.h"
#include <stdlib.h>

MemorySystem gMemSystem;

void* operator new(size_t size, Alignment align, Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}
void* operator new[](size_t size, Alignment align, Allocator allocType)
{
	return gMemSystem.alloc(size, align, allocType);
}

class foo
{
private:
	int bar;
	float baz;
};

int main()
{
	gMemSystem.startup();
	gMemSystem.print();
	foo* pFoo = new(Alignment(4), Allocator::oneFrame) foo();
	delete pFoo;
	gMemSystem.shutdown();
	return 0;
}