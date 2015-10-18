#include "ibThreadBase.h"

#include "ibOS.h"
#include "ibLog.h"
#include "ibUtil.h"

//#include "ibTelemetryManager.h"

namespace
{
	const DWORD MS_VC_EXCEPTION=0x406D1388;
}

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

ibThreadBase::ibThreadBase( ibThreadBase::ThreadCreateFlags flags ):m_result(0)
{
#ifndef NDEBUG
	m_pName = 0;
#endif // NDEBUG

	DWORD sysflags = 0;
	if (flags == kThreadFlag_Suspended)
		sysflags |= CREATE_SUSPENDED;

	m_thread = CreateThread( NULL, 
		                     0,
							 &ibThreadBase::ThreadProc, 
							 (LPVOID)this,
							 flags,
							 &m_threadID
	);

	ibAssert( m_thread != 0 && m_thread != INVALID_HANDLE_VALUE );

	//g_telemetry.AddThread(this);
}

ibThreadBase::~ibThreadBase()
{
	//g_telemetry.RemoveThread(this);
#ifndef NDEBUG
	delete [] m_pName;
#endif
}

void ibThreadBase::Suspend()
{
	SuspendThread( m_thread );
}

void ibThreadBase::Resume()
{
	ResumeThread( m_thread );
}

void ibThreadBase::WaitForThread( unsigned waitTime )
{
	DWORD ret = WaitForSingleObjectEx( m_thread, waitTime, FALSE );
	if ( ret != WAIT_OBJECT_0 )
	{
		ibLog( "WaitForThread failed: 0x%p, %d, (%d ms)\n", m_thread, ret, waitTime );
	}
}

void ibThreadBase::SetThreadName( char* name )
{
#ifndef NDEBUG
	u32 nameLen = strlen(name);
	char* dst = new char[nameLen + 1];
	strcpy(dst, name);
	m_pName = dst;
#endif

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = m_threadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

DWORD CALLBACK ibThreadBase::ThreadProc( void* pParam )
{
	ibThreadBase* pBase = (ibThreadBase*)pParam;
	pBase->Run();
	return pBase->m_result;
}

const char* ibThreadBase::GetThreadName()
{
#ifdef NDEBUG
	return 0;
#else
	return m_pName;
#endif
}
