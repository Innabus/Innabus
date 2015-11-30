#include "ibSemaphore.h"

#include "ibUtil.h"

namespace
{
	HANDLE DupeHandle(HANDLE h)
	{
		HANDLE hOut = INVALID_HANDLE_VALUE;
		if (h != INVALID_HANDLE_VALUE)
		{
			HANDLE hProc = GetCurrentProcess();
			BOOL bRes = DuplicateHandle(hProc, 
				                        h,
										hProc,
										&hOut,
										0,
										FALSE,
										DUPLICATE_SAME_ACCESS);
			ibVerifyMsg(bRes, "Handle duplicationfailed: %d", GetLastError());
		}
		return hOut;
	}
}

ibSemaphore::ibSemaphore( int initialValue, int maxValue ):
m_semaphore( INVALID_HANDLE_VALUE )
{
	ibAssert( maxValue >= 0 );
	ibAssert( initialValue >= 0 && initialValue <= maxValue );

	m_semaphore = CreateSemaphore( NULL, initialValue, maxValue, NULL );
	ibAssert( m_semaphore != 0 && m_semaphore != INVALID_HANDLE_VALUE );
}

ibSemaphore::ibSemaphore( const ibSemaphore& rhs )
{
	m_semaphore = DupeHandle(rhs.m_semaphore);
}

ibSemaphore::~ibSemaphore()
{
	CloseHandle( m_semaphore );
}

ibSemaphore& ibSemaphore::operator=(const ibSemaphore& rhs)
{
	if (&rhs != this)
		m_semaphore = DupeHandle(rhs.m_semaphore);
	return *this;
}

void ibSemaphore::Lock()
{
	DWORD result = WAIT_FAILED;
	while ( result != WAIT_OBJECT_0 )
		result = WaitForSingleObjectEx( m_semaphore, INFINITE, TRUE );
}

void ibSemaphore::Release( int count /*=1*/ )
{
	ReleaseSemaphore( m_semaphore, count, NULL );
}

bool ibSemaphore::TryLock()
{
	DWORD result = WaitForSingleObject( m_semaphore, 0 );
	return result == WAIT_OBJECT_0;
}
