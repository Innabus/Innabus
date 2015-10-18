#include "ibMutex.h"

#include "ibUtil.h"

ibMutex::ibMutex():
m_mutex( INVALID_HANDLE_VALUE )
{
	m_mutex = CreateMutex( NULL, FALSE, NULL );
	ibAssert( m_mutex != 0 && m_mutex != INVALID_HANDLE_VALUE );
}

ibMutex::~ibMutex()
{
	CloseHandle( m_mutex );
}

void ibMutex::Lock()
{
	DWORD result = WAIT_FAILED;
	while ( result != WAIT_OBJECT_0 )
		result = WaitForSingleObject( m_mutex, INFINITE );
}

void ibMutex::Release()
{
	ReleaseMutex( m_mutex );
}

bool ibMutex::TryLock()
{
	DWORD result = WaitForSingleObject( m_mutex, 0 );
	return result == WAIT_OBJECT_0;
}
