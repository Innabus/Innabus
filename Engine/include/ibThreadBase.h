#ifndef IB_THREAD_BASE_H
#define IB_THREAD_BASE_H
#pragma once

#include "ibDataTypes.h"
#include "ibOS.h"

class ibThreadBase
{
public:
	enum ThreadCreateFlags
	{
		kThreadFlag_None,
		kThreadFlag_Suspended
	};

	ibThreadBase( ThreadCreateFlags flags = kThreadFlag_None );
	virtual ~ibThreadBase();

	void Suspend();
	void Resume();

	void SetThreadName(char* name);
	const char* GetThreadName();

	ibOS::ThreadID GetThreadID() { return m_threadID; }

	void WaitForThread(unsigned waitTime = 0xFFFFFFFF /*ms (infinite)*/);

protected:
	virtual void Run() = 0;

	u32 m_result;

private:
#ifdef IB_WINDOWS
	static DWORD CALLBACK ThreadProc( void* pParam );
#endif

	ibOS::Thread m_thread;
	ibOS::ThreadID m_threadID;

#ifndef NDEBUG
	const char* m_pName;
#endif // NDEBUG
};

#endif // IB_THREAD_BASE_H
