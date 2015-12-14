#ifndef IB_RENDER_PASSES_H
#define IB_RENDER_PASSES_H
#pragma once

#include "ibStd.h"

struct ibRenderPassDesc;
class ibRenderPass;

template class IB_RENDER_EXPORT ibVector<ibRenderPass*>;

class IB_RENDER_EXPORT ibRenderPasses
{
public:
	static void Create(ibRenderPassDesc* pDescs, u32 nPasses);
	static ibRenderPasses* Get();

	void Add(ibRenderPass* pPass);
	void Insert(ibRenderPass* pBefore, ibRenderPass* pPass);
	void InsertAfter(ibRenderPass* pAfter, ibRenderPass* pPass);

	void AddNew(ibRenderPassDesc* pDesc);
	void InsertNew(ibRenderPass* pBefore, ibRenderPassDesc* pDesc);
	void InsertAfterNew(ibRenderPass* pAfter, ibRenderPassDesc* pDesc);

	void Remove(ibRenderPass* pPass);

	void Execute();

private:
	ibRenderPasses(ibRenderPassDesc* pDescs, u32 nPasses);
	~ibRenderPasses();

	ibVector<ibRenderPass*> m_renderPasses;
};

#endif // IB_RENDER_PASSES_H
