#include "ibHeap.h"

#include "ibAllocationData.h"
#include "ibMemory.h"
#include "ibLog.h"

//#include "ibTelemetryManager.h"

#define IB_MIN_ALLOC 128
#define IB_ALLOC_FILL 0xCC
#define IB_FREE_FILL 0xDD
#define IB_HEAP_BLOCK_ALIGN 31 // 32 byte align, 31 for maths

ibHeap::ibHeap():
m_region(0),
m_size(0),
#ifdef IB_ENABLE_TELEMETRY
m_used(0),
#endif
#ifndef NDEBUG
m_pDebugName(0),
#endif
m_allocHeap(false)
{
}

ibHeap::ibHeap(const u32 size):
m_region(0),
m_size(size),
#ifdef IB_ENABLE_TELEMETRY
m_used(0),
#endif
#ifndef NDEBUG
m_pDebugName(0),
#endif
m_allocHeap(true)
{
	m_region = new char[size];
	ibAllocationData::CreateBlock(m_region, m_size);
}

ibHeap::ibHeap(void* region, const u32 size):
m_region(region),
m_size(size),
#ifdef IB_ENABLE_TELEMETRY
m_used(0),
#endif
#ifndef NDEBUG
m_pDebugName(0),
#endif
m_allocHeap(false)
{
	ibAllocationData::CreateBlock(m_region, m_size);
}

ibHeap::~ibHeap()
{
	if (m_allocHeap)
		delete [] m_region;
}

bool ibHeap::IsPtrInHeap(void* p)
{
	char* cp = (char*)p;
	return (m_region && cp > m_region && cp < ((char*)m_region + m_size));
}

inline u32 ibHeapHelper_BreakBlock(ibAllocationData* block, u32 minBlockSize, u32 allocSize)
{
	u32 totalAlloc = allocSize;
	// If the block is too small to make two chunks, just take all of it
	if ((block->m_size - totalAlloc) < minBlockSize)
		totalAlloc = block->m_size;
	else // Break the block
	{
		ibAllocationData* nextBlock = ibAllocationData::CreateBlock(((char*)block) + totalAlloc, block->m_size - totalAlloc, block, block->m_next);
		if (block->m_next)
			block->m_next->m_prev = nextBlock;
		block->m_next = nextBlock;
	}
	block->m_size = totalAlloc;
	return totalAlloc;
}

void* ibHeap::Alloc(u32 size)
{
	u32 totalAlloc = ibMax(ibAllocationData::CalculateRequired(size), IB_MIN_ALLOC);
	totalAlloc = (totalAlloc + IB_HEAP_BLOCK_ALIGN) & ~IB_HEAP_BLOCK_ALIGN;
	ibAllocationData* block = (ibAllocationData*)m_region;

	// Start fucking with the chain
	m_lock.Lock();

	// Walk the allocation chain and find the first free (requested == 0) block big enough to hold our alloc
	while (block->m_size < totalAlloc || block->m_requested != 0)
	{
		block = block->m_next;
		if (block == 0)
			ibError("Allocation of %d bytes failed, out of memory or heap fragmented.", totalAlloc);
	}

	block->m_size = ibHeapHelper_BreakBlock(block, IB_MIN_ALLOC, totalAlloc);
	block->m_requested = size;

	// Done fucking with the chain here
	m_lock.Release();
	
	block->SetGuards();

	char* baseData = ((char*)block) + sizeof(ibAllocationData);
#ifdef IB_MEMORY_FILL
	ibMemset(baseData, size, IB_ALLOC_FILL);
#endif

	return baseData;
}

void* ibHeap::AllocHigh(u32 size)
{
	u32 totalAlloc = ibMax(ibAllocationData::CalculateRequired(size), IB_MIN_ALLOC);
	totalAlloc = (totalAlloc + IB_HEAP_BLOCK_ALIGN) & ~IB_HEAP_BLOCK_ALIGN;
	ibAllocationData* block = (ibAllocationData*)m_region;
	ibAllocationData* lastAvailable = 0;

	m_lock.Lock();

	while (block)
	{
		if (block->m_size >= totalAlloc && block->m_requested == 0)
			lastAvailable = block;
		block = block->m_next;
	}
	block = lastAvailable;
	if (!block)
		ibError("Allocation (high) of %d bytes failed, out of memory or heap fragmented.", totalAlloc);
	
	block->m_size = ibHeapHelper_BreakBlock(block, IB_MIN_ALLOC, block->m_size - totalAlloc);
	block = block->m_next;
	block->m_requested = size;

	m_lock.Release();

	block->SetGuards();

	char* baseData = ((char*)block) + sizeof(ibAllocationData);
#ifdef IB_MEMORY_FILL
	ibMemset(baseData, size, IB_ALLOC_FILL);
#endif

	return baseData;
}

void ibHeap::Free(void* pFree)
{
	ibAllocationData* block = (ibAllocationData*)m_region;

	m_lock.Lock();

	while (block)
	{
		if (block <= pFree && (((char*)block) + block->m_size) > pFree)
			break;
		block = block->m_next;
	}
	if (!block)
		ibError("Invalid call to ibHeap::Free; pointer not in heap.");

	block->Verify();
	block->m_requested = 0;

	// Merge blocks block
	if (block->m_next && block->m_next->m_requested == 0)
	{
		block->m_size += block->m_next->m_size;
		block->m_next = block->m_next->m_next;
	}
	if (block->m_prev && block->m_prev->m_requested == 0)
	{
		block->m_prev->m_size += block->m_size;
		block->m_prev->m_next = block->m_next;
		block = block->m_prev;
		block->m_next->m_prev = block;
	}

	m_lock.Release();

#ifdef IB_MEMORY_FILL
	u32 fillSize = block->m_size - sizeof(ibAllocationData);
	char* fillStart = ((char*)block) + sizeof(ibAllocationData);
	ibMemset(fillStart, fillSize, IB_FREE_FILL);
#endif
}

void ibHeap::Check()
{
	u32 totalSize = 0;
	ibAllocationData* block = (ibAllocationData*)m_region;
	m_lock.Lock();
	while (block)
	{
		block->Verify();
		totalSize += block->m_size;
		block = block->m_next;
	}
	m_lock.Release();
	ibVerifyMsg(totalSize == m_size, "Heap block chain corruption, missing blocks.");
}

void ibHeap::Dump()
{
	u32 index = 0;
	ibAllocationData* block = (ibAllocationData*)m_region;
	m_lock.Lock();
	while (block)
	{
		ibLog("%d: 0x%p -- %d (bytes) %s\n",
			index++,
			block,
			block->m_size,
			block->m_requested ? "" : "(Free)");
		block = block->m_next;
	}
	m_lock.Release();
}

u32 ibHeap::GetBlockCount()
{
	u32 count = 0;
	ibAllocationData* block = (ibAllocationData*)m_region;
	m_lock.Lock();
	while (block)
	{
		block = block->m_next;
		++count;
	}
	m_lock.Release();

	return count;
}
