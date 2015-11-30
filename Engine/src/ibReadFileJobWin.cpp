#include "ibReadFileJob.h"

#include "ibFileSystem.h"
#include "ibMemory.h"

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

void CALLBACK ibReadFileJob::ReadFileCompletion(
	DWORD dwErrorCode, 
	DWORD dwBytes,
	LPOVERLAPPED lpOverlapped)
{
	ibReadFileJob* pJob = (ibReadFileJob*)lpOverlapped->hEvent;
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
	pJob->m_fileRef->m_pData = pJob->m_pData;
	pJob->m_fileRef->m_state = kFile_Open;
	pJob->m_state = Finished;
	ibFileSystem::Unlock();
}

ibReadFileJob::Result ibReadFileJob::Execute()
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
			BeginFileRead();
			retCode = kRequeue;
			break;

		case kFile_PendingRead:
			if (m_state == Pending)
				retCode = kRequeue;
			else
				retCode = kFinished; // Discard for other reads
			break;

		case kFile_Opening:
		case kFile_PendingWrite:
			retCode = kRequeue;
			break;
		}
		ibFileSystem::Unlock();		
	}
	return retCode;
}

void ibReadFileJob::BeginFileRead()
{
	ibFileInfo& info = *m_fileRef.Get();
	OVERLAPPED* pOverlapped = new ("ReadFileJob OVERLAPPED") OVERLAPPED;
	ibMemset(pOverlapped, 0, sizeof(OVERLAPPED));
	pOverlapped->hEvent = (HANDLE)this;
	m_pData = new ("File read buffer") u8[info.m_fileSize];
	SetLastError(0); // Because ReadFileEx is retarded
	if (ReadFileEx(info.m_file, m_pData, info.m_fileSize, pOverlapped, &ibReadFileJob::ReadFileCompletion))
	{
		info.m_state = kFile_PendingRead;
		m_state = Pending;
	}

	DWORD dwErr = GetLastError();
	const char* error = ErrorMessage(dwErr);
	ibAssertMsg(dwErr == ERROR_SUCCESS, "ReadFileEx failed (%d): %s\n", GetLastError(), error);
	HeapFree(GetProcessHeap(), 0, (LPVOID)error);
}
