#include "ibRenderTargetManager.h"

#include "ibRenderer.h"
#include "ibRenderDevice.h"

namespace
{
	ibRenderTargetManager* s_pRenderTargetManager;
}

ibRenderTargetManager* ibRenderTargetManager::Get()
{
	if (!s_pRenderTargetManager)
		s_pRenderTargetManager = new (g_engineHeap->AllocHigh(sizeof(ibRenderTargetManager), 
		                                                      "Render target manager"))
															  ibRenderTargetManager;
	return s_pRenderTargetManager;
}

ibRenderTargetManager::ibRenderTargetManager()
{
}

ibRenderTargetManager::~ibRenderTargetManager()
{
}

ibRenderTarget ibRenderTargetManager::GetRenderTarget(u32 key)
{
	ibMap<u32, ibRenderTarget>::iterator iter = m_renderTargets.find(key);
	if (iter != m_renderTargets.end())
		return iter->second;
	return ibRenderTarget();
}

u32 ibRenderTargetManager::CreateRenderTarget(ibRenderTargetParams* pParams, u32 key)
{
	bool bCreate = true;
	if (key < kRenderTarget_Auto)
	{
		ibAssertMsg(m_renderTargets.find(key) == m_renderTargets.end(), "Render target with key already exists");
		if (m_renderTargets.find(key) != m_renderTargets.end())
			bCreate = false;

	}
	else
	{
		key = m_autoKey++;
	}

	if (bCreate)
	{
		bool generateMips = pParams->flags & kRenderTargetParam_Mips;
		ibRenderTarget target = g_renderDevice.CreateRenderTarget(pParams->format, 
																  pParams->width,
																  pParams->height,
																  generateMips ? 0 : 1, // Number of mips
																  generateMips,
																  (pParams->flags & kRenderTargetParam_CPUWrite) != 0);
		if (target.isValid())
			m_renderTargets.insert(std::make_pair(key, target));
		else
			key = (u32)kRenderTarget_Invalid;
	}
	return key;
}

ibRenderDepthStencil ibRenderTargetManager::GetDepthStencil(u32 key)
{
	ibMap<u32, ibRenderDepthStencil>::iterator iter = m_depthStencils.find(key);
	if (iter != m_depthStencils.end())
		return iter->second;
	return ibRenderDepthStencil();
}

u32 ibRenderTargetManager::CreateDepthStencil(ibRenderDepthStencilParams* pParams, ibRenderTarget& rt)
{
	u32 key = m_depthStencils.rbegin()->first + 1;
	ibRenderDepthStencil depthStencil = g_renderDevice.CreateDepthStencil(pParams->format, rt, (pParams->flags & kRenderTargetParam_CPUWrite) != 0);
	m_depthStencils.insert(std::make_pair(key, depthStencil));
	return key;
}

void ibRenderTargetManager::ReleaseRenderTarget(u32 key)
{
	ibAssert(m_renderTargets.find(key) != m_renderTargets.end());
	m_renderTargets.erase(key);
}

void ibRenderTargetManager::ReleaseDepthStencil(u32 key)
{
	ibAssert(m_depthStencils.find(key) != m_depthStencils.end());
	m_depthStencils.erase(key);
}
