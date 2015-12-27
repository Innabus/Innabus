#include "ibRenderPass.h"

#include "ibRenderObject.h"
#include "ibRenderer.h"

ibRenderPass::ibRenderPass():m_owned(false)
{
}

ibRenderPass::ibRenderPass(ibRenderPassDesc* pDesc, bool bOwned):m_owned(bOwned)
{
	ibUnused(pDesc);
}

ibRenderPass::~ibRenderPass()
{
	ibAssertMsg(m_renderObjects.size() == 0, "[RENDER] %d render objects registered to render pass during destruction\n", m_renderObjects.size());
}

void ibRenderPass::RegisterRenderObject(ibRenderObject* pObject)
{
	m_renderObjects.push_back(pObject);
}

void ibRenderPass::UnregisterRenderObject(ibRenderObject* pObject)
{
	ibVector<ibRenderObject*>::iterator iter = std::remove_if(m_renderObjects.begin(),
		                                                      m_renderObjects.end(),
															  [=](ibRenderObject* p){ return p == pObject; });
	ibAssertMsg(iter != m_renderObjects.end(), "Object not found in render pass\n");
	m_renderObjects.erase(iter, m_renderObjects.end());
}

void ibRenderPass::SetCapacity(u32 count, bool bRelease)
{
	if (count < m_renderObjects.size())
		count = m_renderObjects.size();

	m_renderObjects.reserve(count);

	if (bRelease)
	{ 
		if (m_renderObjects.capacity() > count && m_renderObjects.size() < count)
		{
			ibVector<ibRenderObject*> tmp(m_renderObjects);
			m_renderObjects.swap(tmp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// ibRenderPass::Execute
//	Should run on game thread.
///////////////////////////////////////////////////////////////////////////////
void ibRenderPass::Execute()
{
	// Set render pass default state
#ifdef IB_RENDER_PASS_SET_DEFAULT_STATE
#endif // IB_RENDER_PASS_SET_DEFAULT_STATE

	// Cull
	//for (ibRenderObject* pObject : m_renderObjects)
	//{
	//}

	ibRenderCommandList& commandList = g_renderer.GetUpdateCommandList();

	// Render
	for (ibRenderObject* pObject : m_renderObjects)
	{
		pObject->Render(commandList);
		// Verify state restored*
#ifdef IB_RENDER_PASS_VERIFY_STATE_PER_OBJECT
#pragma STATIC_TODO("Verify render pass state after each object")
#endif
	}

	// Verify state restored*
#ifdef IB_RENDER_PASS_VERIFY_STATE
#pragma STATIC_TODO("Verify render pass state after rendering finishes")
#endif
}
