#include "ibLog.h"

#include "ibEngineDefines.h"
#include "ibUtil.h"

#ifdef IB_ENABLE_LOG

#include <stdarg.h>
#include <stdio.h>

void ibLogL( const char* fmt, va_list args )
{
	int len = _vscprintf( fmt, args ) + 1;
	char* buffer = new ("ibLog buffer") char[len];
	_vsnprintf( buffer, len, fmt, args );
	va_end( args );

	innabus::DebugPrint( buffer );
	delete [] buffer;
}

void ibLogH( const char* fmt, ibHeap* pHeap, ... )
{
	va_list args;
	va_start( args, pHeap );

	int len = _vscprintf( fmt, args ) + 1;
	char* buffer = new (pHeap, "ibLog buffer") char[len];
	_vsnprintf( buffer, len, fmt, args );

	innabus::DebugPrint( buffer );
	delete [] buffer;

	va_end( args );
}

void ibLog( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	int len = _vscprintf( fmt, args ) + 1;
	char* buffer = new ("ibLog buffer") char[len];
	_vsnprintf( buffer, len, fmt, args );

	innabus::DebugPrint( buffer );
	delete [] buffer;

	va_end( args );
}

#endif
