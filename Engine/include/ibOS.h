#ifndef IB_OS_H
#define IB_OS_H
#pragma once

#ifdef IB_WINDOWS

#include "ibEngineDefines.h"

#include <Windows.h>

namespace ibOS
{
	typedef HANDLE Thread;
	typedef DWORD ThreadID;

	typedef HANDLE Mutex;
	typedef HANDLE Semaphore;
	typedef CRITICAL_SECTION CriticalSection;

	typedef HWND Window;

	typedef SOCKET Socket;

	typedef HANDLE File;
	typedef FILETIME FileTime;

	u32 GetCoreCount();
	u32 GetProcessId();
}

#endif // IB_WINDOWS

#endif // IB_OS_H
