#ifndef IB_FILEREF_H
#define IB_FILEREF_H
#pragma once

#include "ibReference.h"

class ibFileBase;
template <class T> class ibFile;

enum ibFileState
{
	kFile_Closed,
	kFile_Opening,
	kFile_Open,
	kFile_PendingRead,
	kFile_PendingWrite,
};

struct ibFileInfo
{
	ibOS::File m_file;
	ibOS::FileTime m_openedTime;
	ibFileState m_state;
	u32 m_fileSize;
	bool m_writable;
	void* m_pData;
};

class IB_CORE_EXPORT ibFileRef : public ibReference<ibFileInfo>
{
public:
	ibFileRef();
	~ibFileRef();

	ibFileBase ReadFile();
	void WriteFile();
	
	template <class T>
	ibFile<T> ReadAs() { return ibFile<T>(ReadFile()); }

	bool IsWritable() { return Get()->m_writable; }
	bool IsOpen() { ibFileState state = Get()->m_state; return state >= kFile_Open; }

	void* GetData() { return Get()->m_pData; }
	void* SetData(void* pData, u32 len);
	
	ibFileBase Resize(u32 newSize);
	template <class T>
	ibFile<T> Resize(u32 newSize) { return ibFile<T>(Resize(newSize)); }

	ibFileState GetState() { return Get()->m_state; }

private:
	void ResizeNoCopy(u32 newSize);
};

#endif // IB_FILEREF_H
