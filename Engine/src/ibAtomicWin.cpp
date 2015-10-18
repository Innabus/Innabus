#include "ibAtomic.h"

#include <Windows.h>

unsigned ibAtomic::operator=( int value )
{
	return InterlockedExchange( &m_value, value );
}

unsigned ibAtomic::operator=( volatile ibAtomic& rhs )
{
	return InterlockedExchange( &m_value, rhs.m_value );
}

unsigned ibAtomic::operator++() volatile
{
	return InterlockedIncrement( &m_value );
}

unsigned ibAtomic::operator--() volatile
{
	return InterlockedDecrement( &m_value );
}

unsigned ibAtomic::operator++(int) volatile
{
	unsigned tmp = m_value;
	InterlockedIncrement( &m_value );
	return tmp;
}

unsigned ibAtomic::operator--(int) volatile
{
	unsigned tmp = m_value;
	InterlockedDecrement( &m_value );
	return tmp;
}
