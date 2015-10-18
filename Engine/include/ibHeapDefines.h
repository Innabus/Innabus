// WARNING: This file is included by a forced include; modifying it will trigger a full rebuild
#ifndef IB_HEAP_DEFINES_H
#define IB_HEAP_DEFINES_H

class ibHeap;

enum ibHeapType {
	kHeap_Engine,
	kHeap_Audio,
	kHeap_Misc,

#ifndef NDEBUG
	kHeap_Debug,
#endif 

	kHeap_Count
};

struct ibHeapDesc
{
	u32 Size;
	ibHeap* Heap;
};
extern ibHeapDesc s_Heaps[kHeap_Count];

inline ibHeap* ibGetHeap(ibHeapType type) {	return s_Heaps[type].Heap; }

#define g_engineHeap (ibGetHeap(kHeap_Engine))
#define g_audioHeap  (ibGetHeap(kHeap_Audio))
#define g_miscHeap   (ibGetHeap(kHeap_Misc))

#ifndef NDEBUG
#define g_debugHeap  (ibGetHeap(kHeap_Debug))
#endif

void ibCreateHeaps();

#endif // IB_HEAP_DEFINES_H
