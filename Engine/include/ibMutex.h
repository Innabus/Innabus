#ifndef IB_MUTEX_H
#define IB_MUTEX_H
#pragma once

#include "ibOS.h"

class ibMutex
{
public:
	ibMutex();
	~ibMutex();

	void Lock();
	bool TryLock();
	void Release();

private:
	ibMutex(const ibMutex&);
	ibMutex& operator=(const ibMutex&);

	ibOS::Mutex m_mutex;
};

#endif // IB_MUTEX_H
