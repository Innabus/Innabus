#ifndef IB_RENDERER_H
#define IB_RENDERER_H
#pragma once

class ibRenderDevice;

class IB_RENDER_EXPORT ibRenderer
{
public:
	static void Init();
	static ibRenderer* Get();
	static void Update();

	ibRenderDevice* GetRenderDevice() { return m_pRenderDevice; }

private:
	ibRenderer();
	~ibRenderer();

	ibRenderDevice* m_pRenderDevice;
};

#define g_renderer (*(ibRenderer::Get()))
#define g_renderDevice (*(ibRenderer::Get()->GetRenderDevice()))

#endif // IB_RENDERER_H
