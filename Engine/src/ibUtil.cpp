#include "ibOS.h"

#include "ibLog.h"

namespace innabus
{
	void DebugPrint( char* buffer )
	{
		OutputDebugStringA( buffer );
	}

	void Break()
	{
		DebugBreak();
	}

	void RaiseError( const char* condition, const char* filename, const unsigned line, const char* fmt, ... )
	{
		ibLog( "[ASSERT] %s failed in %s at line %d:\n", condition, filename, line );
		va_list args;
		va_start( args, fmt );
		ibLogL( fmt, args );
		Break();
		va_end( args );
	}
}
