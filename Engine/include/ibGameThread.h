#ifndef IB_GAMETHREAD_H
#define IB_GAMETHREAD_H
#pragma once

#include "ibThreadBase.h"
#include "ibMCP.h"
#include "ibRenderThread.h"
#include "ibSemaphore.h"
#include "ibSystem.h"

class ibGameThread : public ibThreadBase
{
public:
	ibGameThread():m_updateFunc(g_startInfo.gameUpdate), m_semaphore(0, 1) {}
	
	void Advance() { m_semaphore.Release(); }

	void RunLoop() 
	{
		if (m_updateFunc)
		{
			while (!g_mcp.CheckShutdown())
			{
				m_semaphore.Lock();
				(*m_updateFunc)(1.0f / 60.f);
				g_renderThread.Advance();
			}
		}
	}

	void SetUpdateFunc(GameUpdateFunc updateFunc) { m_updateFunc = updateFunc; }

protected:
	void Run() {
		SetThreadName("Gameplay");
		if (g_startInfo.gameInit)
			g_startInfo.gameInit();
		RunLoop();
		g_mcp.Shutdown();
		g_renderThread.Advance();
	}

	GameUpdateFunc m_updateFunc;
	ibSemaphore m_semaphore;
};

#endif // IB_GAMETHREAD_H
