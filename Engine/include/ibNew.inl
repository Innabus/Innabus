#include "ibHeap.h"
#include "ibUtil.h"

// Standard new operates on the misc/default heap
inline void* operator new(std::size_t count)
{
	return g_miscHeap->Alloc(count, "new");
}

inline void* operator new[](std::size_t count)
{
	return g_miscHeap->Alloc(count, "new[]");
}

inline void operator delete(void* p)
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

inline void operator delete[](void* p)
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
inline void* operator new(std::size_t count, ibHeap* pHeap)
{
	return pHeap->Alloc(count, "new (heap)");
}

inline void* operator new[](std::size_t count, ibHeap* pHeap)
{
	return pHeap->Alloc(count, "new (heap) []");
}

inline void operator delete(void* p, ibHeap* pHeap)
{
	ibError("Do not throw exceptions from constructors.");
	pHeap->Free(p);
}

inline void operator delete[](void* p, ibHeap* pHeap)
{
	ibError("Do not throw exceptions from constructors.");
	pHeap->Free(p);
}

inline void* operator new(std::size_t count, const char* pDesc)
{
	ibAssert(pDesc);
	return g_miscHeap->Alloc(count, pDesc);
}

inline void* operator new[](std::size_t count, const char* pDesc)
{
	ibAssert(pDesc);
	return g_miscHeap->Alloc(count, pDesc);
}

inline void operator delete(void* p, const char*)
{
	ibError("Do not throw exceptions from constructors.");
	g_miscHeap->Free(p);
}

inline void operator delete[](void* p, const char*)
{
	ibError("Do not throw exceptions from constructors.");
	g_miscHeap->Free(p);
}

inline void* operator new(std::size_t count, ibHeap* pHeap, const char* pDesc)
{
	ibAssert(pDesc);
	return pHeap->Alloc(count, pDesc);
}

inline void* operator new[](std::size_t count, ibHeap* pHeap, const char* pDesc)
{
	ibAssert(pDesc);
	return pHeap->Alloc(count, pDesc);
}

inline void operator delete(void* p, ibHeap* pHeap, const char*)
{
	ibError("Do not throw exceptions from constructors.");
	pHeap->Free(p);
}

inline void operator delete[](void* p, ibHeap* pHeap, const char*)
{
	ibError("Do not throw exceptions from constructors.");
	pHeap->Free(p);
}

// Placement new
//void* operator new(std::size_t count, void* place) { return place; }
//void* operator new[](std::size_t count, void* place) { return place; }
//void operator delete(void*, void* place) {}
//void operator delete[](void*, void* place) {}
