#include "ibSystem.h"

#include "ibMCP.h"
#include "ibFileSystem.h"
#include "ibEngineSettings.h"

namespace
{
	ibEngineSettings s_engineSettings = { 
		0,   // Display (adapter)
		0,   // Display mode (windowed)
		800, // Width
		600, // Height
		1    // Use window
	};

	const StartupGameInfo* s_pGameInfo;
}

ibEngineSettings* ibSystem::GetSettings()
{
	return &s_engineSettings;
}

const StartupGameInfo* ibSystem::GetStartInfo()
{
	return s_pGameInfo;
}

IB_CORE_EXPORT void SystemStartup(const StartupGameInfo* pGameInfo)
{
	ibAssert(pGameInfo);
	s_pGameInfo = pGameInfo;

	ibCreateHeaps();

	ibSystem::Init();

	ibMCP::Startup(ibOS::GetCoreCount());

	if (g_engineSettings.UseWindow)
		ibSystem::RunLoop();

	g_mcp.ShutdownWait();
	ibFileSystem::Shutdown();
	g_mcp.Destroy();

	ibCheckHeaps();
	ibDumpHeaps();
}
