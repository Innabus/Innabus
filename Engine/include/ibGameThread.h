#ifndef IB_GAMETHREAD_H
#define IB_GAMETHREAD_H
#pragma once

#include "ibThreadBase.h"
#include "ibSystem.h"

extern GameEntryPoint GameMain;

class ibGameThread : public ibThreadBase
{
public:
	void Advance() {}

protected:
	void Run() {
		SetThreadName("Gameplay");
		GameMain();
		g_mcp.Shutdown();
	}
};

#endif // IB_GAMETHREAD_H
