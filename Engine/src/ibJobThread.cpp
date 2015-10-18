#include "ibJobThread.h"

#include "ibJob.h"
#include "ibMCP.h"
#include "ibSemaphore.h"

void ibJobThread::Run()
{
	typedef ibJob::Result Result;
	Result result = Result::kFinished;
	while (result != Result::kKillThread)
	{
		m_pSemaphore->Lock();
		ibJob* pJob = g_mcp.GetPendingJob();

		result = pJob->Execute();

		switch (result)
		{
		case Result::kFinished:
			delete pJob;
			break;

		case Result::kRequeue:
			g_mcp.QueueJob(pJob);
			break;
		}
	}
}
