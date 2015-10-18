#include "ibLog.h"

#include "ibEngineDefines.h"
#include "ibUtil.h"

#ifdef IB_ENABLE_LOG

#include <stdarg.h>
#include <stdio.h>

void ibLogL( const char* fmt, va_list args )
{
	int len = _vscprintf( fmt, args ) + 1;
	char* buffer = new char[len];
	_vsnprintf( buffer, len, fmt, args );
	va_end( args );

	innabus::DebugPrint( buffer );
}

void ibLog( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	int len = _vscprintf( fmt, args ) + 1;
	char* buffer = new char[len];
	_vsnprintf( buffer, len, fmt, args );

	innabus::DebugPrint( buffer );

	va_end( args );
}

#endif
