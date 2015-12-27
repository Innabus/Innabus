#include "ibRenderer.h"

//#include "ibMCP.h"
//#include "ibGameThread.h"

#include "ibLog.h"

#include "ibGameThread.h"
#include "ibMCP.h"
#include "ibRenderDevice.h"
#include "ibSystem.h"

#include "ibColor.h"

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

	m_updateCommandList = 0;
	for (u32 n = 0; n < 2; ++n)
	{
		u8* ptr = g_engineHeap->Alloc<u8>(IB_RENDER_COMMAND_LIST_SIZE, "Render Command List");
		m_commandLists[n].Init(ptr, IB_RENDER_COMMAND_LIST_SIZE);
	}
}

ibRenderer::~ibRenderer()
{
	delete m_pRenderDevice;
}

void ibRenderer::Update()
{
	ibColorF colors[] = {
		ibColorF::SOLID_RED,
		ibColorF::SOLID_YELLOW,
		ibColorF::SOLID_GREEN,
		ibColorF::SOLID_CYAN,
		ibColorF::SOLID_BLUE,
		ibColorF::SOLID_MAGENTA,
	};

	u32 nColors = _countof(colors);
	u32 n = time(0) % nColors;

	// Swap command lists
	s_pRenderer->m_updateCommandList ^= 1;

	// Release game thread
	g_gameThread.Advance();

	if (s_pRenderer->m_pRenderDevice)
	{
		//const float color[4] = { 1.f, 0.f, 1.f, 1.f };
		s_pRenderer->m_pRenderDevice->ClearImmediate(colors[n], 1.f, 0, true);
		s_pRenderer->m_pRenderDevice->Present();
	}

	// Execute commands
	// Present
}

