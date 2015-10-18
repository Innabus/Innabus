#ifndef IB_HEAP_H
#define IB_HEAP_H

#include "ibEngineDefines.h"
#include "ibMutex.h"

class ibHeap
{
public:
	ibHeap(const u32 size);
	ibHeap(void* region, const u32 size);
	virtual ~ibHeap();

	virtual void* Alloc(u32 size);
	virtual void* AllocHigh(u32 size);
	virtual void Free(void*);

	template <typename T> T* Alloc(const u32 size) { return (T*)Alloc(size); }
	template <typename T> T* AllocHigh(const u32 size) { return (T*)AllocHigh(size); }
	
	virtual bool IsPtrInHeap(void*);

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
	u32 GetBlockCount() { return 0; }
#else
	virtual void Check();
	virtual u32 GetBlockCount();
#endif // IB_HEAP_CHECK

#ifdef IB_ENABLE_TELEMETRY
public:
	u32 GetHeapSize() { return m_size; }
	u32 GetHeapUsed() { return m_used; }
	void* GetHeapBase() { return m_region; }

private:
	u32 m_used;
#endif

protected:
	ibHeap();

private:
	// No copy/assign for heaps
	ibHeap(const ibHeap& rhs);
	ibHeap& operator=(const ibHeap& rhs);
	
	void* m_region;
	u32 m_size;

	ibMutex m_lock;
	bool m_allocHeap;

#ifndef NDEBUG
	const char* m_pDebugName;
#endif // !NDEBUG
};

#endif // IB_HEAP_H
