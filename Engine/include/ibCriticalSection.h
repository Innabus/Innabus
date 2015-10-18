#ifndef IB_CRITICAL_SECTION_H
#define IB_CRITICAL_SECTION_H
#pragma once

#include "ibEngineDefines.h"

#ifndef IB_PLATFORM_HAS_CRITICAL_SECTION

// If the platform doesn't have a critical section natively, just
// use the stronger mutex solution
#include "ibMutex.h"
typedef ibMutex ibCriticalSection;

#else

#include "ibOS.h"

class ibCriticalSection
{
public:
	ibCriticalSection();
	~ibCriticalSection();

	void Lock();
	bool TryLock();
	void Release();

private:
	ibOS::CriticalSection m_criticalSection;
};

#endif // !IB_PLATFORM_HAS_CRITICAL_SECTION

#endif // IB_CRITICAL_SECTION_H
