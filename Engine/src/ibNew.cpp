#include "ibHeap.h"

#include "ibUtil.h"

// Standard new operates on the misc/default heap
void* operator new(std::size_t count)
{
	return g_miscHeap->Alloc(count);
}

void* operator new[](std::size_t count)
{
	return g_miscHeap->Alloc(count);
}

void operator delete(void* p)
{
	for (u32 n = 0; n < kHeap_Count; ++n)
	{
		ibHeap* pHeap = ibGetHeap((ibHeapType)n);
		if (pHeap->IsPtrInHeap(p))
		{
			pHeap->Free(p);
			break;
		}
	}
}

void operator delete[](void* p)
{
	for (u32 n = 0; n < kHeap_Count; ++n)
	{
		ibHeap* pHeap = ibGetHeap((ibHeapType)n);
		if (pHeap->IsPtrInHeap(p))
		{
			pHeap->Free(p);
			break;
		}
	}
}

// The new operators taking a heap pointer allocate in those heaps
void* operator new(std::size_t count, ibHeap* pHeap)
{
	return pHeap->Alloc(count);
}

void* operator new[](std::size_t count, ibHeap* pHeap)
{
	return pHeap->Alloc(count);
}

void operator delete(void* p, ibHeap* pHeap)
{
	ibError("Do not throw exceptions from constructors.");
	pHeap->Free(p);
}

void operator delete[](void* p, ibHeap* pHeap)
{
	ibError("Do not throw exceptions from constructors.");
	pHeap->Free(p);
}

// Placement new
//void* operator new(std::size_t count, void* place) { return place; }
//void* operator new[](std::size_t count, void* place) { return place; }
//void operator delete(void*, void* place) {}
//void operator delete[](void*, void* place) {}
