#ifndef IB_ALLOCATION_DATA_H
#define IB_ALLOCATION_DATA_H

#include "ibEngineDefines.h"

#ifdef IB_ALLOC_GUARDS
#include "ibUtil.h"
#define IB_ALLOC_GUARD_VALUE 0xDEADBEEF
#endif // IB_ALLOC_GUARDS

class ibAllocationData
{
public:
	u32 m_size; // The real block size, including guards, header and padding
	u32 m_requested; // The size of allocation requested with Alloc/AllocHigh, 0 if available
	ibAllocationData* m_prev; // Previous block
	ibAllocationData* m_next; // Next block

#ifdef IB_ALLOC_GUARDS
	char padding[64 - (2 * sizeof(u32)) - (sizeof(void*) * 3) - sizeof(u32[8])];
	u32* m_footerGuard;
	u32 m_headerGuard[8];

	void SetGuards()
	{
		m_footerGuard = (u32*)(((char*)this) + sizeof(ibAllocationData) + m_requested);
		for (u32 i = 0; i < 8; ++i)
		{
			m_headerGuard[i] = IB_ALLOC_GUARD_VALUE;
			m_footerGuard[i] = IB_ALLOC_GUARD_VALUE;
		}
	}

	void Verify()
	{
		if (m_requested)
		{
			for (int i = 0; i < 8; ++i)
			{
				ibVerifyMsg(m_headerGuard[i] == IB_ALLOC_GUARD_VALUE, "Allocation underwrite at 0x%p", this + sizeof(this));
				ibVerifyMsg(m_footerGuard[i] == IB_ALLOC_GUARD_VALUE, "Allocation overwrite at 0x%p", this + sizeof(this));
			}
		}
	}
#else
	void SetGuards() {}
	void Verify() {}
#endif // IB_ALLOC_GUARDS

	static ibAllocationData* CreateBlock(
		void* base,
		u32 size,
		ibAllocationData* prev = 0,
		ibAllocationData* next = 0);

	static u32 CalculateRequired(u32 size);
};

inline ibAllocationData* ibAllocationData::CreateBlock(
	void* base,
	u32 size,
	ibAllocationData* prev,
	ibAllocationData* next)
{
	ibAllocationData* block = (ibAllocationData*)base;
	block->m_size = size;
	block->m_requested = 0;
	block->m_prev = prev;
	block->m_next = next;

	// No guards needed because theres no actual alloc here
	block->m_footerGuard = 0;

	return block;
}

inline u32 ibAllocationData::CalculateRequired(u32 size)
{
	u32 total = size + sizeof(ibAllocationData);
#ifdef IB_ALLOC_GUARDS
	total += sizeof(u32) * 8; // For the footer
#endif // IB_ALLOC_GUARDS
	return total;
}

#endif // IB_ALLOCATION_DATA_H
