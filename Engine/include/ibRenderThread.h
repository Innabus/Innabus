#ifndef IB_RENDER_THREAD_H
#define IB_RENDER_THREAD_H
#pragma once

#include "ibThreadBase.h"
#include "ibSemaphore.h"

class ibRenderThread : public ibThreadBase
{
public:
	ibRenderThread();

	void Advance() { m_semaphore.Release(); }

protected:
	void Run();

private:
	ibSemaphore m_semaphore;
};

#endif // IB_RENDER_THREAD_H
