#ifndef IB_SYSTEM_H
#define IB_SYSTEM_H

void SystemStartup();

class ibSystem
{
public:
	static void* SystemAlloc(u32 sizeBytes);
};

#endif // IB_SYSTEM_H
