#ifndef IB_OPEN_FILE_JOB_H
#define IB_OPEN_FILE_JOB_H
#pragma once

#include "ibJob.h"

#include "ibFileRef.h"

class ibOpenFileJob : public ibJob
{
public:
	ibOpenFileJob(const char* filename, bool bWrite, const ibFileRef& ref):m_fileRef(ref), m_writable(bWrite)
	{
		m_filename = new ("OpenFleJob path copy") char[strlen(filename) + 1];
		strcpy(m_filename, filename);
		m_fileRef.Get()->m_state = kFile_Opening;
	}

	~ibOpenFileJob()
	{
		delete [] m_filename;
	}

	typedef ibJob::Result Result;
	Result Execute();

private:
	ibFileRef m_fileRef;
	char* m_filename;
	bool m_writable;
};

#endif IB_OPEN_FILE_JOB_H
