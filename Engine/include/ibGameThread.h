#ifndef IB_GAMETHREAD_H
#define IB_GAMETHREAD_H
#pragma once

#include "ibThreadBase.h"
#include "ibSystem.h"

extern GameEntryPoint GameMain;

class ibGameThread : public ibThreadBase
{
public:
	ibGameThread():m_updateFunc(0), m_semaphore(0, 1) {}
	
	void Advance() { m_semaphore.Release(); }

	void RunLoop() 
	{
		while (!g_mcp.CheckShutdown())
		{
			if (m_updateFunc)
				(*m_updateFunc)(1.0f / 60.f);
		}
	}

	void SetUpdateFunc(GameUpdateFunc updateFunc) { m_updateFunc = updateFunc; }

protected:
	void Run() {
		SetThreadName("Gameplay");
		GameMain();
		g_mcp.Shutdown();
	}

	GameUpdateFunc m_updateFunc;
	ibSemaphore m_semaphore;
};

#endif // IB_GAMETHREAD_H
