#ifndef IB_SYSTEM_H
#define IB_SYSTEM_H

typedef int (*GameEntryPoint)();
typedef int (*GameUpdateFunc)(float);

struct StartupGameInfo
{
	GameEntryPoint gameMain;
};

struct ibEngineSettings;

IB_EXPORT void SystemStartup(StartupGameInfo* pGameInfo);

class ibSystem
{
public:
	static ibEngineSettings* GetSettings();

	static void Init();
	static int RunLoop();

	static void* SystemAlloc(u32 sizeBytes);
};

#define g_engineSettings (*(ibSystem::GetSettings()))
#endif // IB_SYSTEM_H
