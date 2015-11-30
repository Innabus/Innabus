#include "ibHeapDefines.h"

#include "ibHeap.h"
#include "ibMemory.h"
#include "ibSystem.h"

// This needs to be overloaded for every system, but for now assume we just have 1 gig
#define AVAIL_MEGS 1024
#define SYSTEM_MEMORY_SIZE 1024 * 1024 * AVAIL_MEGS

#define HEAP_ENGINE_SIZE 40
#define HEAP_AUDIO_SIZE 8

#ifndef NDEBUG
#define HEAP_DEBUG_SIZE 10
#else
#define HEAP_DEBUG_SIZE 0
#endif // NDEBUG

#define HEAP_MISC_SIZE (AVAIL_MEGS - HEAP_ENGINE_SIZE - HEAP_AUDIO_SIZE - HEAP_DEBUG_SIZE)

ibHeapDesc s_Heaps[kHeap_Count] = {
	{ HEAP_ENGINE_SIZE << 20, 0 }, 	// Engine heap
	{ HEAP_AUDIO_SIZE << 20,  0 }, 	// Audio heap
	{ HEAP_MISC_SIZE << 20,   0 },  // Misc heap

#ifndef NDEBUG
	{ HEAP_DEBUG_SIZE << 20, 0 },
#endif 
};

namespace
{
	const char* s_HeapDescs[kHeap_Count] = {
		"Engine",
		"Audio",
		"Misc",
#ifndef NDEBUG
		"Debug"
#endif
	};
}

void ibCreateHeaps()
{
	void* baseMemory = ibSystem::SystemAlloc(SYSTEM_MEMORY_SIZE);
	ibHeap setupHeap(baseMemory, SYSTEM_MEMORY_SIZE);
	u32 sectionSize = sizeof(ibHeap) * kHeap_Count;
	ibHeap* pBootstrap = setupHeap.AllocHigh<ibHeap>(sectionSize);
	u32 heapOffset = 0;
	for (u32 n = 0; n < kHeap_Count; ++n)
	{
		void* placement = &pBootstrap[n];
		new (placement) ibHeap((void*)((char*)baseMemory + heapOffset), s_Heaps[n].Size);
		heapOffset += s_Heaps[n].Size;
	}

	ibHeap* pHeaps = pBootstrap->AllocHigh<ibHeap>(sectionSize, "Heaps");
	ibMemcpy(pHeaps, pBootstrap, sectionSize);
	ibMemset(pBootstrap, 0, sectionSize);
	
	for (u32 n = 0; n < kHeap_Count; ++n)
	{
		s_Heaps[n].Heap = &pHeaps[n];
		s_Heaps[n].Heap->SetName(s_HeapDescs[n]);
	}

	// setupHeap deletes without calling destructors or affecting its memory
}

void IB_EXPORT ibCheckHeaps()
{
	for (u32 n = 0; n < kHeap_Count; ++n)
		s_Heaps[n].Heap->Check();
}

void IB_EXPORT ibDumpHeaps()
{
	for (u32 n = 0; n < kHeap_Count; ++n)
		s_Heaps[n].Heap->Dump();
}