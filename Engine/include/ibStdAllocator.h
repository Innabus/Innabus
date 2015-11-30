#ifndef IB_STD_ALLOCATOR_H
#define IB_STD_ALLOCATOR_H
#pragma once

#include <cstddef>

#include "ibHeap.h"

template <class T>
class ibAllocator
{
public:
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef const T& const_reference;
	typedef void* void_pointer;
	typedef const void * const_void_pointer;

	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	template <class U>
	struct rebind {
		typedef ibAllocator<U> other;
	};

	ibAllocator() {}
	template<class T> ibAllocator(const ibAllocator<T>&) {}

	pointer allocate(size_type n) { return (T*)g_miscHeap->Alloc(sizeof(T) * n, "ibAllocator"); }
	void deallocate(T* p, size_type) { g_miscHeap->Free(p); }
	
	pointer address (reference value) const { return &value; }
	const_pointer address (const_reference value) const { return &value; }

	size_type max_size() const { return g_miscHeap->Size() / sizeof(T); }

	void construct(pointer p, const T& value) { new ((void*)p)T(value); }
	void destroy(pointer ptr) { ptr; ptr->~T(); }
};

template <class T, class U>
bool operator== (const ibAllocator<T>&, const ibAllocator<U>&) { return true; }
template <class T, class U>
bool operator!= (const ibAllocator<T>&, const ibAllocator<U>&) { return false; }

#endif // IB_STD_ALLOCATOR_H
