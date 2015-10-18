#ifndef IB_REFERENC_H
#define IB_REFERENC_H
#pragma once

#include "ibReferenceExternal.h"

#include "ibMCP.h"

template <typename T>
class ibReference
{
public:
	ibReference():m_pRef(0) {
		m_pRef = g_mcp.CreateRef(new T());
		AddRef();
	}

	explicit ibReference(T* pObj):m_pRef(0) {
		m_pRef = g_mcp.CreateRef(pObj);
		AddRef();
	}

	ibReference(const ibReference& rhs)	{
		m_pRef = rhs.m_pRef;
		AddRef();
	}

	ibReference& operator=(const ibReference& rhs) {
		Release();
		m_pRef = rhs.m_pRef;
		AddRef();
		return *this;
	}

	virtual ~ibReference() { Release(); }

	u32 GetRefCount() { return m_pRef->GetCount(); }

	T* Get() { return (T*)m_pRef->Get(); }

	T& operator*() { return *Get(); }
	const T& operator*() const { return *Get(); }

	T* operator->() { return Get(); }
	const T* operator->() const { return Get(); }

private:
	void AddRef() {
		if (m_pRef)
			m_pRef->AddRef();
	}

	void Release()	{
		if (m_pRef && !m_pRef->Release())
		{
			delete Get();
			g_mcp.RemoveRef(m_pRef);
		}
	}

	ibReferenceExternal* m_pRef;
};

#endif // IB_REFERENC_H
