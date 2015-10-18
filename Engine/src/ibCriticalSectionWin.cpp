#include "ibCriticalSection.h"

ibCriticalSection::ibCriticalSection()
{
	InitializeCriticalSection( &m_criticalSection );
}

ibCriticalSection::~ibCriticalSection()
{
	DeleteCriticalSection( &m_criticalSection );
}

void ibCriticalSection::Lock()
{
	EnterCriticalSection( &m_criticalSection );
}

void ibCriticalSection::Release()
{
	LeaveCriticalSection( &m_criticalSection );
}

bool ibCriticalSection::TryLock()
{
	BOOL result = TryEnterCriticalSection( &m_criticalSection );
	return result != 0;
}
