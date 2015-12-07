#ifndef DN_LOG_H
#define DN_LOG_H
#pragma once

#include "ibEngineDefines.h"
#include <stdarg.h>

#if defined IB_ENABLE_LOG

IB_CORE_EXPORT void ibLog( const char* fmt, ... );
IB_CORE_EXPORT void ibLogL( const char* fmt, va_list args );
IB_CORE_EXPORT void ibLogH( const char* fmt, ibHeap* pHeap, ... );

#else
#define ibLog( ... )
#define ibLogL( ... )
#define ibLogH( ... )
#endif // DN_ENABLE_LOG

#endif // DN_LOG_H
