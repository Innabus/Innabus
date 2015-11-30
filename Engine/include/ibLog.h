#ifndef DN_LOG_H
#define DN_LOG_H
#pragma once

#include "ibEngineDefines.h"
#include <stdarg.h>

#if defined IB_ENABLE_LOG

void ibLog( const char* fmt, ... );
void ibLogL( const char* fmt, va_list args );
void ibLogH( const char* fmt, ibHeap* pHeap, ... );

#else
#define ibLog( ... )
#define ibLogL( ... )
#define ibLogH( ... )
#endif // DN_ENABLE_LOG

#endif // DN_LOG_H
