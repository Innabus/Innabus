#ifndef IB_FORCED_INCLUDE_H
#define IB_FORCED_INCLUDE_H

#undef IB_EXPORT
#ifdef _USRDLL
#define IB_EXPORT __declspec(dllexport)
#else
#define IB_EXPORT __declspec(dllimport)
#endif // _USRDLL

#ifdef CORE_DLL
#define IB_CORE_EXPORT IB_EXPORT
#else
#define IB_CORE_EXPORT __declspec(dllimport)
#endif

#ifdef RENDER_DLL
#define IB_RENDER_EXPORT IB_EXPORT
#else
#define IB_RENDER_EXPORT __declspec(dllimport)
#endif

#define _CRT_SECURE_NO_WARNINGS

#include "ibDataTypes.h"
#include "ibEngineDefines.h"

#ifdef __cplusplus
#include "ibNew.h"
#include "ibUtil.h"
#endif // __cplusplus

#endif // IB_FORCED_INCLUDE_H
