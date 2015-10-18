// WARNING: This file is a forced include, modifying it will trigger a full rebuild.
#ifndef IB_NEW_H
#define IB_NEW_H

#include <cstddef>

#include "ibHeapDefines.h"

// Standard new operates on the misc/default heap
void* operator new(std::size_t count);
void* operator new[](std::size_t count);
void operator delete(void*);
void operator delete[](void*);

// The new operators taking a heap pointer allocate in those heaps
void* operator new(std::size_t count, ibHeap* pHeap);
void* operator new[](std::size_t count, ibHeap* pHeap);
void operator delete(void*, ibHeap* pHeap);
void operator delete[](void*, ibHeap* PHeap);

// Placement new
void* operator new(std::size_t count, void* place);
void* operator new[](std::size_t count, void* place);
void operator delete(void*, void* place);
void operator delete[](void*, void* place);

#endif // IB_NEW_H