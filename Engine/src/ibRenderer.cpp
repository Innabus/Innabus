#include "ibRenderer.h"

//#include "ibMCP.h"
//#include "ibGameThread.h"

#include "ibLog.h"

#include "ibGameThread.h"
#include "ibMCP.h"
#include "ibRenderDevice.h"
#include "ibSystem.h"

#include <ctime>

namespace
{
	ibRenderer* s_pRenderer;
}

void ibRenderer::Init()
{
	ibAssert(s_pRenderer == 0);
	s_pRenderer = new (g_engineHeap->AllocHigh(sizeof(ibRenderer), "ibRenderer")) ibRenderer;
}

void ibRenderer::Shutdown()
{
	ibAssert(s_pRenderer);
	delete s_pRenderer;
}

ibRenderer* ibRenderer::Get()
{
	return s_pRenderer;
}

ibRenderer::ibRenderer()
{
	if (!ibSystem::GetMainWindow())
		return;

	m_pRenderDevice = new (g_engineHeap->AllocHigh(sizeof(ibRenderDevice), "ibRenderDevice")) ibRenderDevice;
}

ibRenderer::~ibRenderer()
{
	delete m_pRenderDevice;
}

void ibRenderer::Update()
{
	float colors[][4] = {
		{ 1.f, 0.f, 0.f, 1.f },
		{ 1.f, 0.f, 1.f, 1.f },
		{ 1.f, 1.f, 0.f, 1.f },
		{ 0.f, 1.f, 0.f, 1.f },
		{ 0.f, 1.f, 1.f, 1.f },
		{ 0.f, 0.f, 1.f, 1.f },
	};
	u32 nColors = _countof(colors);
	u32 n = time(0) % nColors;

	// Swap command lists

	// Release game thread
	g_gameThread.Advance();

	if (s_pRenderer->m_pRenderDevice)
	{
		//const float color[4] = { 1.f, 0.f, 1.f, 1.f };
		s_pRenderer->m_pRenderDevice->ClearImmediate(colors[n], 1.f, 0, true);
		s_pRenderer->m_pRenderDevice->Present();
	}

	// Execute commands
}
