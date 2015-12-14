#include "ibRenderPasses.h"

#include "ibRenderPass.h"

namespace
{
	static ibRenderPasses* s_pRenderPasses;
}

ibRenderPasses::ibRenderPasses(ibRenderPassDesc* pDescs, u32 nPasses)
{
	m_renderPasses.reserve(nPasses);
	for (u32 n = 0; n < nPasses; ++n)
	{
		AddNew(pDescs);
	}
}

ibRenderPasses::~ibRenderPasses()
{
	ibVector<ibRenderPass*>::iterator iter = std::remove_if(m_renderPasses.begin(),
		                                                    m_renderPasses.end(),
															[](ibRenderPass* p) { return p->IsOwned(); });
	ibVector<ibRenderPass*>::iterator working;
	for (working = iter; working != m_renderPasses.end(); ++working)
	{
		delete *working;
	}
	m_renderPasses.erase(iter, m_renderPasses.end());

	ibAssertMsg(m_renderPasses.size() == 0, "[RENDER] %d unowned render passes registered during destruction\n", m_renderPasses.size());
}

void ibRenderPasses::Create(ibRenderPassDesc* pDescs, u32 nPasses)
{
	ibAssert(!s_pRenderPasses);
	s_pRenderPasses = new ibRenderPasses(pDescs, nPasses);
}

ibRenderPasses* ibRenderPasses::Get()
{
	ibAssert(s_pRenderPasses);
	return s_pRenderPasses;
}

void ibRenderPasses::Add(ibRenderPass* pPass)
{
	ibAssert(pPass);
	m_renderPasses.push_back(pPass);
}

void ibRenderPasses::Insert(ibRenderPass* pBefore, ibRenderPass* pPass)
{
	ibVector<ibRenderPass*>::iterator iter = std::find(m_renderPasses.begin(), m_renderPasses.end(), pBefore);
	m_renderPasses.insert(iter, pPass);
}

void ibRenderPasses::InsertAfter(ibRenderPass* pAfter, ibRenderPass* pPass)
{
	ibVector<ibRenderPass*>::iterator iter = std::find(m_renderPasses.begin(), m_renderPasses.end(), pAfter);
	if (iter != m_renderPasses.end())
		++iter;
	m_renderPasses.insert(iter, pPass);
}

void ibRenderPasses::AddNew(ibRenderPassDesc* pDesc)
{
	m_renderPasses.push_back(new ("Render pass") ibRenderPass(pDesc, true));
}

void ibRenderPasses::InsertNew(ibRenderPass* pBefore, ibRenderPassDesc* pDesc)
{
	ibRenderPass* pPass = new ("Render pass") ibRenderPass(pDesc, true);
	Insert(pBefore, pPass);
}

void ibRenderPasses::InsertAfterNew(ibRenderPass* pAfter, ibRenderPassDesc* pDesc)
{
	ibRenderPass* pPass = new ("Render pass") ibRenderPass(pDesc, true);
	InsertAfter(pAfter, pPass);
}

void ibRenderPasses::Remove(ibRenderPass* pPass)
{
	ibVector<ibRenderPass*>::iterator iter = std::remove_if(m_renderPasses.begin(),
		                                                      m_renderPasses.end(),
															  [=](ibRenderPass* p){ return p == pPass; });
	ibAssertMsg(iter != m_renderPasses.end(), "Pass not found in render pass\n");
	m_renderPasses.erase(iter, m_renderPasses.end());
}

void ibRenderPasses::Execute()
{
	// Debug clear
#ifdef IB_RENDER_DEBUG_CLEAR
#endif

	for (ibRenderPass* pPass : m_renderPasses)
		pPass->Execute();

	// Present
}
