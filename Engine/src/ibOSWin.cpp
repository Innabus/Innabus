#include "ibOS.h"

u32 ibOS::GetCoreCount()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

u32 ibOS::GetProcessId()
{
	return GetCurrentProcessId();
}
