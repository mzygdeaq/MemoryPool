#include "Alloc.h"
#include "MemoryMgr.hpp"
#include <stdlib.h>

void * operator new(size_t size)
{
	return MemoryMgr::Instance().allocMem(size);
}

void operator delete(void* p)
{
	MemoryMgr::Instance().freeMem(p);
}

void * operator new[](size_t size)
{
	return MemoryMgr::Instance().allocMem(size);
}
void operator delete[](void* p)
{
	MemoryMgr::Instance().freeMem(p);
}

void* mem_alloc(size_t size)
{
	return malloc(size);
}

void mem_free(void* p)
{
	free(p);
}