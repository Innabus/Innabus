#ifndef IB_MCP_H
#define IB_MCP_H

#include "ibReferenceExternal.h"
#include "ibCriticalSection.h"
#include "ibSemaphore.h"
#include "ibStd.h"

class ibJob;
class ibGameThread;
class ibRenderThread;
class ibThreadBase;
class ibTelemetryManager;

class ibMCP
{
public:
	static void Startup(u32 nMaxAuxThreads);
	static void Shutdown();
	static void ShutdownWait();
	static void Destroy();
	static ibMCP* Get();

	bool CheckShutdown() { return !!(m_flags & MCPFlags::kShutdown); }

	// Threads
	ibGameThread* GetGameplayThread();
	ibRenderThread* GetRenderThread();
	ibThreadBase* GetAuxThread(u32 index);

	// Jobs
	void QueueJob(ibJob* pJOb);
	ibJob* GetPendingJob();

	// References
	ibReferenceExternal* CreateRef(void* pObj);
	void RemoveRef(ibReferenceExternal* pRef);
	void AcquireReferenceLock();
	void ReleaseReferenceLock();
	u32 GetReferenceCount();
	ibReferenceExternal* GetReferenceByIndex(u32 index);

private:
	ibMCP();
	~ibMCP();

	enum MCPFlags {
		kStarted  = 0x01,
		kShutdown = 0x02,
	};

	void Init(u32 threadCount);

	u32 m_flags;

	u32 m_maxAuxThreads;

	ibGameThread* m_pGameThread;
	ibRenderThread* m_pRenderThread;
	ibThreadBase** m_pAuxThreads;

	// Jobs
	ibCriticalSection m_jobQueueCS;
	ibSemaphore m_jobSemaphore;
	ibQueue<ibJob*> m_jobQueue;

	// Reference tracking
	ibCriticalSection m_referenceLock;
	ibList<ibReferenceExternal> m_references;

	// External telemetry
#ifdef IB_ENABLE_TELEMETRY
	ibTelemetryManager* m_pTelemetry;
#endif
};

#define g_mcp (*ibMCP::Get())
#define g_gameThread (*(ibMCP::Get()->GetGameplayThread()))
#define g_renderThread (*(ibMCP::Get()->GetRenderThread()))

#endif // IB_MCP_H
