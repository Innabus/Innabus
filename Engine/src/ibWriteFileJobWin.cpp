#include "ibWriteFileJob.h"

#include "ibFileSystem.h"
#include "ibMemory.h"

#include "ibLog.h"

namespace
{
	LPCSTR ErrorMessage( DWORD error ) 
	{
		char* lpMsgBuf;

		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpMsgBuf,
			0, NULL );

		return lpMsgBuf;
	}
}

void CALLBACK ibWriteFileJob::WriteFileCompletion(
	DWORD dwErrorCode, 
	DWORD dwBytes,
	LPOVERLAPPED lpOverlapped)
{
	ibWriteFileJob* pJob = (ibWriteFileJob*)lpOverlapped->hEvent;
	if (dwErrorCode == 0)
	{
		if (!GetOverlappedResult(pJob->m_fileRef.Get()->m_file, lpOverlapped, &dwBytes, FALSE))
			dwErrorCode = GetLastError();
	}

	if (dwErrorCode)
	{
		const char* error = ErrorMessage(dwErrorCode);
		ibError("Reading file failed (%d): %s\n", dwErrorCode, error);
		HeapFree(GetProcessHeap(), 0, (LPVOID)error);
	}
	delete lpOverlapped;
	
	ibFileSystem::Lock();
	pJob->m_state = Finished;
	pJob->m_fileRef->m_state = kFile_Open;
	ibFileSystem::Unlock();
}

ibWriteFileJob::Result ibWriteFileJob::Execute()
{
	Result retCode = kFinished;
	if (m_state != Finished)
	{
		ibFileSystem::Lock();
		switch (m_fileRef.GetState())
		{
		case kFile_Closed:
			break;

		case kFile_Open:
			BeginFileWrite();
			retCode = kRequeue;
			break;

		case kFile_PendingWrite:
			if (m_state == Pending)
				retCode = kRequeue;
			else
				retCode = kFinished; // Discard for other reads
			break;

		case kFile_Opening:
		case kFile_PendingRead:
			retCode = kRequeue;
			break;
		}
		ibFileSystem::Unlock();

	}
	return retCode;
}

void ibWriteFileJob::BeginFileWrite()
{
	ibFileInfo& info = *m_fileRef.Get();
	OVERLAPPED* pOverlapped = new ("WriteFileJob OVERLAPPED") OVERLAPPED;
	ibMemset(pOverlapped, 0, sizeof(OVERLAPPED));
	pOverlapped->hEvent = (HANDLE)this;
	SetLastError(0);
	if (WriteFileEx(info.m_file, info.m_pData, info.m_fileSize, pOverlapped, WriteFileCompletion))
	{
		info.m_state = kFile_PendingWrite;
		m_state = Pending;
	}

	DWORD dwErr = GetLastError();
	const char* error = ErrorMessage(dwErr);
	ibAssertMsg(dwErr == ERROR_SUCCESS, "WriteFileEx failed (%d): %s\n", GetLastError(), error);
	HeapFree(GetProcessHeap(), 0, (LPVOID)error);
}
