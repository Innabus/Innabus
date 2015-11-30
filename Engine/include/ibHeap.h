#ifndef IB_HEAP_H
#define IB_HEAP_H

#include "ibEngineDefines.h"
#include "ibMutex.h"

class ibHeap
{
public:
	// Create a heap and allocate size bytes for it
	// Note: This uses new for allocation, meaning the
	// heap created is a sub-region of the misc heap.
	// The initial block has prev = 0, next = 0
	ibHeap(const u32 size);

	// Create a heap with the specified region and
	// size.  The alloc flag is not set.
	// The initial block has prev = 0, next = 0
	ibHeap(void* region, const u32 size);

	// Destroy the heap.  If the alloc flag is set
	// this deletes the controlled region.
	virtual ~ibHeap();

	// Alloc allocates size bytes for use, but uses a
	// total of size + sizeof(ibAllocationData) bytes
	// in the heap.  If the allocation is less than
	// IB_MIN_ALLOC the number of bytes allocated is
	// increased.  If IB_ALLOC_GAURDS are enabled an
	// extra 64 bytes (32 each) for the header and footer
	// are allocated to provide overflow/underflow
	// and corruption detection.
	// Debug strings *MUST* point to static data
	virtual void* Alloc(u32 size, const char* pDebugString = 0);
	virtual void* AllocHigh(u32 size, const char* pDebugString = 0);
	virtual void Free(void*);

	template <typename T>
	T* Alloc(const u32 size, const char* pDebugString = 0) {
		return (T*)Alloc(size, pDebugString);
	}
	template <typename T>
	T* AllocHigh(const u32 size, const char* pDebugString = 0) {
		return (T*)AllocHigh(size, pDebugString); 
	}
	
	virtual bool IsPtrInHeap(void*);

	u32 Size() const { return m_size; }

#ifndef NDEBUG
public:
	const char* GetName() { return m_pDebugName; }
	void SetName(const char* pName) { m_pDebugName = pName; }
#endif // ~NDEBUG

#ifndef IB_HEAP_ENABLE_DUMP
	void Dump() {}
#else
	virtual void Dump();
#endif

#ifndef IB_HEAP_CHECK
	void Check() {}
#else
	virtual void Check();
#endif // IB_HEAP_CHECK
	virtual u32 GetBlockCount();

#ifdef IB_ENABLE_TELEMETRY
public:
	//u32 GetHeapSize() { return m_size; }
	u32 GetHeapUsed() { return m_used; }
	void* GetHeapBase() { return m_region; }

private:
	u32 m_used;
#endif

protected:
	// Initiialized the heap without any size or region,
	// and with the alloc flag set to false.
	ibHeap();

private:
	// No copy/assign for heaps
	ibHeap(const ibHeap& rhs);
	ibHeap& operator=(const ibHeap& rhs);
	
	void GetFreeSpaceAndBlocks(u32* pSpace, u32* pBlocks);

	void* m_region;
	u32 m_size;

	ibMutex m_lock;
	bool m_allocHeap;

#ifndef NDEBUG
	const char* m_pDebugName;
#endif // !NDEBUG
};

#endif // IB_HEAP_H
