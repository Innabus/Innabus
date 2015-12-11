#include "ibRenderThread.h"

#include "ibGameThread.h"
#include "ibMCP.h"
#include "ibSystem.h"

ibRenderThread::ibRenderThread():m_semaphore(0, 1)
{
}

void ibRenderThread::Run()
{
	SetThreadName("Render Thread");

	ibAssert(g_startInfo.renderInit);
	ibAssert(g_startInfo.renderUpdate);

	g_startInfo.renderInit();

	while (!g_mcp.CheckShutdown())
	{
		// Wait for advance
		m_semaphore.Lock();
		// Update renderer
		if (!g_mcp.CheckShutdown()) // Skip if the shutdown happened while we were locked
			g_startInfo.renderUpdate();
	}
	
	g_gameThread.Advance();

	if (g_startInfo.renderShutdown)
		g_startInfo.renderShutdown();
}
