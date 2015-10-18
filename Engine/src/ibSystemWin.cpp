#include <Windows.h>
#include "ibSystem.h"

#include "ibUtil.h"

void* s_systemHeapPtr = 0;

void* ibSystem::SystemAlloc(u32 sizeBytes)
{
	ibAssertMsg(s_systemHeapPtr == 0, "Attempt to allocate system heap twice.");
	s_systemHeapPtr = HeapAlloc(GetProcessHeap(), 0, sizeBytes);
	ibAssertMsg(s_systemHeapPtr, "Failed to allocate system heap.");
	return s_systemHeapPtr;
}
