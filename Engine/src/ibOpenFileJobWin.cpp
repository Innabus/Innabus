#include "ibOpenFileJob.h"

ibOpenFileJob::Result ibOpenFileJob::Execute()
{
	ibFileInfo* fi = m_fileRef.Get();
	DWORD access = GENERIC_READ;
	if (m_writable)
	{
		DWORD attrs = GetFileAttributesA(m_filename);
		if (!(attrs & FILE_ATTRIBUTE_READONLY))
			access |= GENERIC_WRITE;
		else
			m_writable = false;
	}
	fi->m_file = CreateFileA(m_filename, access, 0, NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	if (fi->m_file != INVALID_HANDLE_VALUE)
	{
		GetSystemTimeAsFileTime(&fi->m_openedTime);
		LARGE_INTEGER li = { 0 };
		GetFileSizeEx(fi->m_file, &li);
		fi->m_fileSize = li.LowPart;

		if (access & GENERIC_WRITE)
			fi->m_writable = true;

		fi->m_state = kFile_Open;
	}
	else
	{
		fi->m_state = kFile_Closed;
		DWORD dwErr = GetLastError();
		// TODO: Add actual error diagnostics
		//switch (dwErr)
		//{
		//default:
		//	break;
		//}

		innabus::RaiseError("CreateFile() != INVALID_HANDLE_VALUE", __FILE__, __LINE__, "@err == %d", dwErr);
	}
	return kFinished;
}
