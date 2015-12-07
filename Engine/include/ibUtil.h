#ifndef IB_UTIL_H
#define IB_UTIL_H
#pragma once

#include "ibEngineDefines.h"

namespace innabus
{
	IB_CORE_EXPORT void DebugPrint( char* buffer );
	IB_CORE_EXPORT void RaiseError( const char* condition, const char* filename, const unsigned line, const char* fmt, ... );
	IB_CORE_EXPORT void Break();
}

#ifdef IB_ASSERT_ENABLE
	#define ibAssert( cond ) if (!(cond)) innabus::RaiseError( #cond, __FILE__, __LINE__, "" )
	#define ibAssertMsg( cond, ... ) if (!(cond)) innabus::RaiseError( #cond, __FILE__, __LINE__, __VA_ARGS__ )
#else
	#define ibAssert(...)
	#define ibAssertMsg(...)
#endif // IB_ASSERT_ENABLE

#define ibError( ... ) innabus::RaiseError( "Error", __FILE__, __LINE__, __VA_ARGS__ )
#define ibVerify( cond ) if (!(cond)) innabus::RaiseError( #cond, __FILE__, __LINE__, "" )
#define ibVerifyMsg( cond, ... ) if (!(cond)) innabus::RaiseError( #cond, __FILE__, __LINE__, __VA_ARGS__ )

#define ibUnused( x ) x

#define SAFE_RELEASE(x) if ((x)) { x->Release(); x = 0; }

#define ibMin(x, y) ((x < y) ? x : y)
#define ibMax(x, y) ((x < y) ? y : x)

#endif // IB_UTIL_H
