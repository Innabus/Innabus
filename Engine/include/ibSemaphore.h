#ifndef IB_SEMAPHORE_H
#define IB_SEMAPHORE_H
#pragma once

#include "ibOS.h"

class ibSemaphore
{
public:
	ibSemaphore( int initialValue, int maxValue );
	ibSemaphore( const ibSemaphore& rhs );
	~ibSemaphore();

	ibSemaphore& operator=(const ibSemaphore& rhs);

	void Lock();
	bool TryLock();
	void Release( int count = 1 );

private:
	ibOS::Semaphore m_semaphore;
};

#endif IB_SEMAPHORE_H
