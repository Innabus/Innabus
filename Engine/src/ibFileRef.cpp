#include "ibFileRef.h"

#include "ibFile.h"
#include "ibFileSystem.h"
#include "ibReadFileJob.h"
#include "ibWriteFileJob.h"

#include "ibMemory.h"

ibFileRef::ibFileRef()
{
	ibMemset(Get(), 0, sizeof(ibFileInfo));
#ifdef IB_WINDOWS
	Get()->m_file = INVALID_HANDLE_VALUE;
#endif // IB_WINDOWS
}

ibFileRef::~ibFileRef()
{
	// If there are two references left, this is the last
	// external ref: 1 for this, 1 for the internal file map
	if (GetRefCount() == 2 && Get()->m_state != kFile_Closed)
	{
		ibFileInfo* pInfo = Get();
		delete [] pInfo->m_pData;
		pInfo->m_pData = 0;
		g_fileSystem.CloseFile(this);
	}
}

ibFileBase ibFileRef::ReadFile()
{
	if (IsOpen() && !Get()->m_pData)
	{
		g_mcp.QueueJob(new ("Read File Job") ibReadFileJob(*this));
	}
	return ibFileBase(*this);
}

void ibFileRef::WriteFile()
{
	if (IsWritable())
	{
		g_mcp.QueueJob(new ("Write File Job") ibWriteFileJob(*this));
	}
}

void* ibFileRef::SetData(void* pData, u32 len)
{
	ResizeNoCopy(len);
	ibMemcpy(GetData(), pData, len);
	return GetData();
}

ibFileBase ibFileRef::Resize(u32 newSize)
{
	if (newSize <= Get()->m_fileSize)
	{
		if (!GetData())
		{
			Get()->m_pData = new ("File data buffer") char[newSize];
			memset(GetData(), 0, newSize);
		}
	}
	else
	{
		u32 oldSize = Get()->m_fileSize;
		char* newData = new ("File data buffer") char[newSize];
		void* oldData = Get()->m_pData;

		ibMemcpy(newData, Get()->m_pData, oldSize);
		ibMemset(newData + oldSize, 0, newSize - oldSize);
		Get()->m_pData = newData;
		delete [] oldData;
	}

	Get()->m_fileSize = newSize;
	return ibFileBase(*this);
}

void ibFileRef::ResizeNoCopy(u32 newSize)
{
	if (newSize <= Get()->m_fileSize)
	{
		Get()->m_fileSize = newSize;
		if (!GetData())
		{
			Get()->m_pData = new ("File data buffer") char[newSize];
			memset(GetData(), 0, newSize);
		}
	}
	else
	{
		char* newData = new ("File data buffer") char[newSize];
		void* oldData = GetData();
		Get()->m_pData = newData;
		delete [] oldData;
	}
}
