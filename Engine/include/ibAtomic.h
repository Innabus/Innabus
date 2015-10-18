#ifndef IB_ATOMIC_H
#define IB_ATOMIC_H
#pragma once

class ibAtomic
{
public:
	unsigned operator=( int value );
	unsigned operator=( volatile ibAtomic& rhs );

	unsigned operator++() volatile;
	unsigned operator--() volatile;

	unsigned operator++(int) volatile;
	unsigned operator--(int) volatile;

private:
	volatile unsigned m_value;
};

#endif // IB_ATOMIC_H
