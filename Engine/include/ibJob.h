#ifndef IB_JOB_H
#define IB_JOB_H
#pragma once

class ibJob
{
public:
	enum Result {
		kFinished,
		kRequeue,
		kKillThread,
	};

	virtual ~ibJob() {}
	virtual Result Execute() = 0;
};

#endif // IB_JOB_H
