#ifndef IB_JOB_THREAD_H
#define IB_JOB_THREAD_H
#pragma once

#include "ibThreadBase.h"

class ibSemaphore;

class ibJobThread : public ibThreadBase
{
public:
	ibJobThread(ibSemaphore* pSema, ibThreadBase::ThreadCreateFlags flags):ibThreadBase(flags), m_pSemaphore(pSema) {}
	~ibJobThread();

protected:
	virtual void Run();

private:
	ibSemaphore* m_pSemaphore;
};

#endif // IB_JOB_THREAD_H
