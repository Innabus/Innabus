#ifndef IB_READ_FILE_JOB_H
#define IB_READ_FILE_JOB_H
#pragma once

#include "ibJob.h"

#include "ibFileRef.h"

class ibReadFileJob : public ibJob
{
public:
	ibReadFileJob(const ibFileRef& ref):m_fileRef(ref), m_pData(0), m_state(Starting) {}

	typedef ibJob::Result Result;
	Result Execute();

private:
	void BeginFileRead();

	ibFileRef m_fileRef;
	u8* m_pData;

	enum {
		Starting,
		Pending,
		Finished,
	} m_state;

#ifdef IB_WINDOWS
	static void CALLBACK ReadFileCompletion(DWORD, DWORD, LPOVERLAPPED);
#endif // IB_WINDOWS
};

#endif // IB_READ_FILE_JOB_H
