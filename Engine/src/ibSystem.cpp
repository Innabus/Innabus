#include "ibSystem.h"

#include "ibMCP.h"

GameEntryPoint GameMain = 0;

void SystemStartup(StartupGameInfo* pGameInfo)
{
	GameMain = pGameInfo->gameMain;

	ibCreateHeaps();

	ibMCP::Startup(ibOS::GetCoreCount());

	g_mcp.ShutdownWait();
}
