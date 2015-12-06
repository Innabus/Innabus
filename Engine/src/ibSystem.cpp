#include "ibSystem.h"

#include "ibMCP.h"
#include "ibFileSystem.h"
#include "ibEngineSettings.h"

GameEntryPoint GameMain = 0;

namespace
{
	ibEngineSettings s_engineSettings = { 0, 0, 800, 600, 1 };
}

ibEngineSettings* ibSystem::GetSettings()
{
	return &s_engineSettings;
}

void SystemStartup(StartupGameInfo* pGameInfo)
{
	GameMain = pGameInfo->gameMain;

	ibCreateHeaps();

	ibMCP::Startup(ibOS::GetCoreCount());

	ibSystem::Init();
	ibSystem::RunLoop();

	g_mcp.ShutdownWait();
	ibFileSystem::Shutdown();
	g_mcp.Destroy();

	ibCheckHeaps();
	ibDumpHeaps();
}
