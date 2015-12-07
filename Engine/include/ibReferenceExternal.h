#ifndef IB_REFERENCE_EXTERNAL_H
#define IB_REFERENCE_EXTERNAL_H
#pragma once

#include "ibUtil.h"

class IB_CORE_EXPORT ibReferenceExternal
{
public:
	explicit ibReferenceExternal(void* pObj):m_object(pObj), m_count(0) {}
	~ibReferenceExternal() { ibAssert(m_count == 0); }

	u32 AddRef() { return ++m_count; }
	u32 Release() { return --m_count; }
	u32 GetCount() const { return m_count; }

	void* Get() { return m_object; }

private:
	void* m_object;
	u32 m_count;
};

#endif // IB_REFERENCE_EXTERNAL_H
