#ifndef IB_SYSTEM_H
#define IB_SYSTEM_H

typedef int (*GameEntryPoint)();
typedef int (*GameUpdateFunc)(float);
typedef void (*RenderInitFunc)();
typedef void (*RenderUpdateFunc)();
typedef void (*RenderShutdownFunc)();

struct StartupGameInfo
{
	GameEntryPoint gameInit;
	GameUpdateFunc gameUpdate;
	RenderInitFunc renderInit;
	RenderUpdateFunc renderUpdate;
	RenderShutdownFunc renderShutdown;
};

struct ibEngineSettings;

IB_CORE_EXPORT void SystemStartup(const StartupGameInfo* pGameInfo);

class IB_CORE_EXPORT ibSystem
{
public:
	static ibEngineSettings* GetSettings();
	static const StartupGameInfo* GetStartInfo();

	static void Init();
	static int RunLoop();

	static void* SystemAlloc(u32 sizeBytes);

	static ibOS::Window GetMainWindow();
};

#define g_engineSettings (*(ibSystem::GetSettings()))
#define g_startInfo (*(ibSystem::GetStartInfo()))

#endif // IB_SYSTEM_H
