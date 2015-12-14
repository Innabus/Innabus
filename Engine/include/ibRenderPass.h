#ifndef IB_RENDER_PASS_H
#define IB_RENDER_PASS_H
#pragma once

class ibRenderObject;

#include "ibStd.h"

template class IB_RENDER_EXPORT ibVector<ibRenderObject*>;

struct IB_RENDER_EXPORT ibRenderPassDesc
{
};

class IB_RENDER_EXPORT ibRenderPass
{
public:
	ibRenderPass();
	ibRenderPass(ibRenderPassDesc*, bool bOwned = false);
	~ibRenderPass();

	bool IsOwned() const { return m_owned; }

	void RegisterRenderObject(ibRenderObject* pObject);
	void UnregisterRenderObject(ibRenderObject* pObject);

	void SetCapacity(u32 count, bool bRelease = false);

	void Execute();

private:
	ibRenderPass(const ibRenderPass&); // No impl
	ibRenderPass& operator= (const ibRenderPass&); // no impl;

	ibVector<ibRenderObject*> m_renderObjects;
	const bool m_owned;
};

#endif // IB_RENDER_PASS_H
