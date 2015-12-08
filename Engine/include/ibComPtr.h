#ifndef IB_COM_PTR_H
#define IB_COM_PTR_H
#pragma once

#ifdef IB_TRACK_COM_REFERENCES
#include "ibLog.h"
#endif

template <class T>
class ComPtr
{
public:
	ComPtr();
	explicit ComPtr(T*);
	explicit ComPtr(T&);
	ComPtr(const ComPtr& rhs);
	~ComPtr();

	unsigned AddRef();
	unsigned Release();

	ComPtr<T>& operator=( T* t );
	ComPtr<T>& operator=( T& t );
	ComPtr<T>& operator=( const ComPtr<T>& rhs );

	T* operator*() { return m_ptr; }
	T* operator->() { return m_ptr; }
	
	const T* operator*() const { return m_ptr; }
	const T* operator->() const { return m_ptr; }

	operator T**() { 
#ifdef IB_TRACK_COM_REFERENCES
		ibLog( "[COM] Returning com pointer reference for init\n" );
#endif
		return &m_ptr; 
	}

	operator void**() {
#ifdef IB_TRACK_COM_REFERENCES
		ibLog( "[COM] Returning com pointer reference as void** for init\n" );
#endif
		return (void**)&m_ptr;
	}

private:
	T* m_ptr;
};

template <class T>
ComPtr<T>::ComPtr():m_ptr(0) {}

template <class T>
ComPtr<T>::ComPtr(T* t):m_ptr(t) 
{ 
		AddRef(); 
#ifdef IB_TRACK_COM_REFERENCES
		ibLog( "[COM] Creating com pointer to 0x%p\n", t );
#endif
}

template <class T>
	ComPtr<T>::ComPtr(T& t):m_ptr(&t) { /* No AddRef */ }

template <class T>
ComPtr<T>::ComPtr(const ComPtr& rhs):m_ptr(rhs.m_ptr) { AddRef(); }

template <class T>
ComPtr<T>::~ComPtr() { Release(); }

template <class T>
ComPtr<T>& ComPtr<T>::operator=( T* t )
{
	if ( m_ptr ) Release();
	m_ptr = t;
	AddRef();
#ifdef IB_TRACK_COM_REFERENCES
	ibLog( "[COM] Creating com pointer to 0x%p\n", t );
#endif
	return *this;
}

template <class T>
ComPtr<T>& ComPtr<T>::operator=( T& t )
{
	if ( m_ptr ) Release();
	m_ptr = &t;
	/* No AddRef */
#ifdef IB_TRACK_COM_REFERENCES
	ibLog( "[COM] Creating com pointer to 0x%p\n", &t );
#endif
	return *this;
}

template <class T>
ComPtr<T>& ComPtr<T>::operator=( const ComPtr<T>& rhs )
{
	if ( m_ptr ) Release();
	m_ptr = rhs.m_ptr;
	AddRef();
	return *this;
}

template <class T>
unsigned ComPtr<T>::AddRef() { 
	unsigned cnt = 0;
	if ( m_ptr ) cnt = m_ptr->AddRef();
	return cnt; 
}

template <class T>
unsigned ComPtr<T>::Release() {
	unsigned ret = 0;
	if ( m_ptr )
	{
		ret = m_ptr->Release();
#ifdef IB_TRACK_COM_REFERENCES
		if (!ret)
			ibLog( "[COM] Releasing COM object at 0x%p\n", *m_ptr );
#endif
	}
	m_ptr = 0;
	return ret;
}

#endif // IB_COM_PTR_H
