#ifndef IB_RENDER_TARGET_MANAGER_H
#define IB_RENDER_TARGET_MANAGER_H
#pragma once

#include "ibRenderTarget.h"
#include "ibRenderDepthStencil.h"

#include "ibStd.h"

enum ibRenderTargetKeys
{
	kRenderTarget_Screen,
	kRenderTarget_Diffuse,

	kRenderTarget_Count,
	kRenderTarget_User,
	kRenderTarget_Auto = 0x8000000,
	kRenderTarget_Invalid = 0xFFFFFFFF
};

const u32 kRenderTargetParam_Mips = 1;
const u32 kRenderTargetParam_CPUWrite = 2;

struct ibRenderTargetParams
{
	u32 width;
	u32 height;
	ibFormat format;
	u32 flags;
};

struct ibRenderDepthStencilParams
{
	ibFormat format;
	u32 flags;
};

class ibRenderTargetManager
{
public:
	static ibRenderTargetManager* Get();

	ibRenderTarget GetRenderTarget(u32 key);
	u32 CreateRenderTarget(ibRenderTargetParams* pParams, u32 key = kRenderTarget_Auto);

	ibRenderDepthStencil GetDepthStencil(u32 key);
	u32 CreateDepthStencil(ibRenderDepthStencilParams* pParams, ibRenderTarget& renderTarget);

	void ReleaseRenderTarget(u32);
	void ReleaseDepthStencil(u32);

private:
	ibRenderTargetManager();
	~ibRenderTargetManager();

	void Init();

	u32 m_autoKey;
	ibMap<u32, ibRenderTarget> m_renderTargets;
	ibMap<u32, ibRenderDepthStencil> m_depthStencils;
};

#define g_renderTargetManger (*ibRenderTargetManager::Get())

#endif // IB_RENDER_TARGET_MANAGER_H
