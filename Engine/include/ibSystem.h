#ifndef IB_SYSTEM_H
#define IB_SYSTEM_H

typedef int (*GameEntryPoint)();
typedef int (*GameUpdateFunc)(float);
typedef void (*RenderInitFunc)();
typedef void (*RenderUpdateFunc)();

struct StartupGameInfo
{
	GameEntryPoint gameInit;
	GameUpdateFunc gameUpdate;
	RenderInitFunc renderInit;
	RenderUpdateFunc renderUpdate;
};

struct ibEngineSettings;

IB_CORE_EXPORT void SystemStartup(const StartupGameInfo* pGameInfo);

class ibSystem
{
public:
	static ibEngineSettings* GetSettings();
	static const StartupGameInfo* GetStartInfo();

	static void Init();
	static int RunLoop();

	static void* SystemAlloc(u32 sizeBytes);
};

#define g_engineSettings (*(ibSystem::GetSettings()))
#define g_startInfo (*(ibSystem::GetStartInfo()))

#endif // IB_SYSTEM_H
