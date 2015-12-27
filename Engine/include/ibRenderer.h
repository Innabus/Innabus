#ifndef IB_RENDERER_H
#define IB_RENDERER_H
#pragma once

class ibRenderDevice;

#include "ibRenderCommandList.h"

class IB_RENDER_EXPORT ibRenderer
{
public:
	static void Init();
	static void Shutdown();
	static ibRenderer* Get();
	static void Update();

	ibRenderDevice* GetRenderDevice() { return m_pRenderDevice; }

	// The update list is used by gameplay to create render commands,
	// the render list is used by the render thread to execute them.
	ibRenderCommandList& GetUpdateCommandList() { return m_commandLists[m_updateCommandList]; }
	ibRenderCommandList& GetRenderCommandList() { return m_commandLists[m_updateCommandList ^ 1]; }

private:
	ibRenderer();
	~ibRenderer();

	ibRenderDevice* m_pRenderDevice;
	ibRenderCommandList m_commandLists[2];
	u32 m_updateCommandList;
};

#define g_renderer (*(ibRenderer::Get()))
#define g_renderDevice (*(ibRenderer::Get()->GetRenderDevice()))

#endif // IB_RENDERER_H
