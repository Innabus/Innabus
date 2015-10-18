#include "ibMCP.h"

#include "ibCriticalSection.h"
#include "ibGameThread.h"
#include "ibKillJob.h"
#include "ibSemaphore.h"
#include "ibThreadBase.h"
#include "ibJobThread.h"

// #include "ibTelemetryManager.h"

#include "ibUtil.h"

#define MAX_PENDING_JOBS 100

namespace
{
	ibMCP* s_pMCPInst = 0;
}

void ibMCP::Startup(u32 nMaxAuxThreads)
{
	ibVerifyMsg(s_pMCPInst == 0, "Error: Startup called more than once.");
	s_pMCPInst = new (g_engineHeap) ibMCP;
	s_pMCPInst->Init(nMaxAuxThreads);
}

void ibMCP::Shutdown()
{
	ibAssert(s_pMCPInst);
	s_pMCPInst->m_flags |= kShutdown;
}

void ibMCP::ShutdownWait()
{
	s_pMCPInst->m_pGameThread->WaitForThread();
	//s_pMCPInst->m_pRenderThread->WaitForThread();
	if ( s_pMCPInst->m_pAuxThreads )
	{
		for ( u32 i = 0; i < s_pMCPInst->m_maxAuxThreads; ++i )
			s_pMCPInst->QueueJob( new (g_engineHeap) ibKillJob );

		for ( u32 i = 0; i < s_pMCPInst->m_maxAuxThreads; ++i )
		{
			s_pMCPInst->m_pAuxThreads[i]->WaitForThread();
			delete s_pMCPInst->m_pAuxThreads[i];
		}
		delete [] s_pMCPInst->m_pAuxThreads;
		s_pMCPInst->m_pAuxThreads = 0;
	}
}

void ibMCP::Destroy()
{
#ifdef IB_ENABLE_TELEMETRY
	ibTelemetryManager::Destroy();
#endif // IB_ENABLE_TELEMETRY

	delete s_pMCPInst;
	s_pMCPInst = 0;
}

ibMCP* ibMCP::Get()
{
	return s_pMCPInst;
}

ibMCP::ibMCP():m_flags(0), m_jobSemaphore(0, MAX_PENDING_JOBS)
{
}

ibMCP::~ibMCP()
{
}

void ibMCP::Init(u32 threadCount)
{
	ibAssert(m_flags == 0);

#ifdef IB_ENABLE_TELEMETRY
	m_pTelemetry = ibTelemetryManager::Create();

	g_telemetry.AddHeap(g_engineHeap);
	g_telemetry.AddHeap(g_audioHeap);
	g_telemetry.AddHeap(g_miscHeap);

#ifndef NDEBUG
	g_telemetry.AddHeap(g_debugHeap);
#endif

#endif // IB_ENABLE_TELEMETRY

	m_maxAuxThreads = threadCount;
	m_pGameThread = 0;
	m_pRenderThread = 0;
	m_pAuxThreads = 0;

	m_pAuxThreads = new (g_engineHeap) ibThreadBase*[m_maxAuxThreads];
	for (u32 n = 0; n < m_maxAuxThreads; ++n)
	{
		// Create aux threads suspended and immediately resume so that ibJobThread's ctor can set the sema
		// pointer without a race for thread entry
		m_pAuxThreads[n] = new (g_engineHeap) ibJobThread(&m_jobSemaphore, ibThreadBase::kThreadFlag_Suspended);
		m_pAuxThreads[n]->SetThreadName("Job thread");
		m_pAuxThreads[n]->Resume();
	}

	m_pGameThread = new (g_engineHeap) ibGameThread;
	//m_pRenderThread = new (g_engineHeap) ibRenderThread;

	//m_pGameThread->Advance();
}

ibGameThread* ibMCP::GetGameplayThread()
{
	return m_pGameThread;
}

ibRenderThread* ibMCP::GetRenderThread()
{
	return m_pRenderThread;
}

ibThreadBase* ibMCP::GetAuxThread(u32 index)
{
	ibThreadBase* ret = 0;
	if (index < m_maxAuxThreads && m_pAuxThreads)
	{
		ret = m_pAuxThreads[index];
	}
	return ret;
}

void ibMCP::QueueJob(ibJob* pJob)
{
	ibAssert(m_jobQueue.size() < MAX_PENDING_JOBS);
	m_jobQueueCS.Lock();
	m_jobQueue.push(pJob);
	m_jobQueueCS.Release();
	m_jobSemaphore.Release();
}

ibJob* ibMCP::GetPendingJob()
{
	ibAssert(m_jobQueue.size());

	m_jobQueueCS.Lock();
	ibJob* ret = m_jobQueue.front();
	m_jobQueue.pop();
	m_jobQueueCS.Release();

	return ret;
}

ibReferenceExternal* ibMCP::CreateRef(void* pObject)
{
	m_referenceLock.Lock();
	m_references.push_back(ibReferenceExternal(pObject));
	ibReferenceExternal* pRet = &*m_references.rbegin();
	m_referenceLock.Release();
	return pRet;
}

void ibMCP::RemoveRef(ibReferenceExternal* pRef)
{
	ibAssertMsg(pRef->GetCount() == 0, "Removing a reference with an invalid ref count");
	std::list<ibReferenceExternal>::iterator b, e = m_references.end();
	for (b = m_references.begin(); b != e; ++b)
	{
		if (&*b == pRef)
		{
			m_references.erase(b);
			break;
		}
	}
}

void ibMCP::AcquireReferenceLock()
{
	m_referenceLock.Lock();
}

void ibMCP::ReleaseReferenceLock()
{
	m_referenceLock.Release();
}

u32 ibMCP::GetReferenceCount()
{
	return m_references.size();
}

ibReferenceExternal* ibMCP::GetReferenceByIndex(u32 index)
{
	std::list<ibReferenceExternal>::iterator i = m_references.begin();
	std::advance(i, index);
	return &*i;
}
