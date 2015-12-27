#include "ibRenderDevice.h"

ibRenderDevice::ibRenderDevice()
{
	PlatformInit();
	BlendReset();
	BlendCommit();
	DepthStencilReset();
	DepthStencilCommit();
}

ibRenderDevice::~ibRenderDevice()
{
}

void ibRenderDevice::SetFlags(u32 flags)
{
	m_flags |= flags;
}

void ibRenderDevice::ClearFlags(u32 flags)
{
	m_flags &= ~flags;
}

u32 ibRenderDevice::CheckFlags(u32 flags)
{
	return m_flags & flags;
}

u32 ibRenderDevice::CheckAllFlags(u32 flags)
{
	return (m_flags & flags) == flags;
}
