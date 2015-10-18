#ifndef IB_KILL_JOB_H
#define IB_KILL_JOB_H
#pragma once

#include "ibJob.h"

class ibKillJob : public ibJob
{
	typedef ibJob::Result Result;
	Result Execute() { return Result::kKillThread; }
};

#endif // IB_KILL_JOB_H
