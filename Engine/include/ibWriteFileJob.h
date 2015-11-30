#ifndef IB_WRITE_FILE_JOB_H
#define IB_WRITE_FILE_JOB_H
#pragma once

#include "ibJob.h"

#include "ibFileRef.h"

class ibWriteFileJob : public ibJob
{
public:
	ibWriteFileJob(const ibFileRef& ref):m_fileRef(ref), m_pData(0), m_state(Starting) {}

	typedef ibJob::Result Result;
	Result Execute();

private:
	void BeginFileWrite();

	ibFileRef m_fileRef;
	u8* m_pData;

	enum {
		Starting,
		Pending,
		Finished,
	} m_state;

#ifdef IB_WINDOWS
	static void CALLBACK WriteFileCompletion(DWORD, DWORD, LPOVERLAPPED);
#endif // IB_WINDOWS
};

#endif // IB_READ_FILE_JOB_H
