#ifndef IB_FORCED_INCLUDE_H
#define IB_FORCED_INCLUDE_H

#undef IB_EXPORT
#ifdef _USRDLL
#define IB_EXPORT __declspec(dllexport)
#else
#define IB_EXPORT __declspec(dllimport)
#endif // _USRDLL

#define _CRT_SECURE_NO_WARNINGS

#include "ibDataTypes.h"
#include "ibEngineDefines.h"
#include "ibUtil.h"

#endif // IB_FORCED_INCLUDE_H
