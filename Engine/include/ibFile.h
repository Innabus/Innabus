#ifndef IB_FILE_H
#define IB_FILE_H
#pragma once

#include "ibFileRef.h"

class IB_EXPORT ibFileBase
{
public:
	ibFileBase(const ibFileRef& ref):m_fileRef(ref) {}
	ibFileBase(const ibFileBase& rhs):m_fileRef(rhs.m_fileRef) {}
	virtual ~ibFileBase() {}

	bool IsReady() { return m_fileRef.GetData() && (m_fileRef.GetState() == kFile_Open); }
	bool IsWritable() { return m_fileRef.IsWritable(); }
	
	void WriteFile() { m_fileRef.WriteFile(); }

	void* GetData() { return m_fileRef.GetData(); }

	u32 Size() const { return m_fileRef.Get()->m_fileSize; }

	void* operator->() { return GetData(); }

protected:
	ibFileRef m_fileRef;

private:
	ibFileBase& operator= (const ibFileBase&); // No impl
};

template <class T>
class ibFile : public ibFileBase
{
public:
	ibFile(const ibFileRef& ref):ibFileBase(ref) {}
	ibFile(const ibFileBase& base):ibFileBase(base) {}
	ibFile(const ibFile& rhs):ibFileBase(rhs) {}
	~ibFile() {}

	T operator*() { return *(T*)GetData(); }
	T* operator->() { return (T*)GetData(); }

private:
	ibFile<T>& operator=(const ibFile<T>& rhs); // No impl
};

#endif // IB_FILE_H
