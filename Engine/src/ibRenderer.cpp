#include "ibRenderer.h"

//#include "ibMCP.h"
//#include "ibGameThread.h"

#include "ibLog.h"

#include "ibRenderDevice.h"

namespace
{
	ibRenderer* s_pRenderer;
}

void ibRenderer::Init()
{
	ibAssert(s_pRenderer == 0);
	s_pRenderer = new (g_miscHeap->AllocHigh(sizeof(ibRenderer), "ibRenderer")) ibRenderer;
}

ibRenderer* ibRenderer::Get()
{
	return s_pRenderer;
}

ibRenderer::ibRenderer()
{
	m_pRenderDevice = new (g_miscHeap->AllocHigh(sizeof(ibRenderDevice), "ibRenderDevice")) ibRenderDevice;
}

ibRenderer::~ibRenderer()
{
}

void ibRenderer::Update()
{
	ibLog("Render Update\n");

	// Swap command lists
	// Release game thread
	//g_gameThread.Advance();

	// Execute commands
}
