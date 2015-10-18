#ifndef IB_SYSTEM_H
#define IB_SYSTEM_H

typedef int (*GameEntryPoint)();

struct StartupGameInfo
{
	GameEntryPoint gameMain;
};

IB_EXPORT void SystemStartup(StartupGameInfo* pGameInfo);

class ibSystem
{
public:
	static void* SystemAlloc(u32 sizeBytes);
};

#endif // IB_SYSTEM_H
