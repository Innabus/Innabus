#ifndef IB_COLOR_H
#define IB_COLOR_H
#pragma once

#include "ibDataTypes.h"

class ibColor;
class ibColorF;

class ibColor
{
public:
	explicit ibColor( u32 col ); // 0xrrggbbaa
	ibColor( u8 red, u8 green, u8 blue, u8 alpha );
	explicit ibColor( u8* pBytes );
	explicit ibColor( const ibColorF& );
	ibColor( const ibColor& );

	union
	{
		struct { u8 r, g, b, a; } rgba;
		u32 color;
	};

	static ibColor SOLID_BLACK;   // 0x000000
	static ibColor SOLID_RED;     // 0xFF0000
	static ibColor SOLID_GREEN;   // 0x00FF00
	static ibColor SOLID_BLUE;    // 0x0000FF
	static ibColor SOLID_YELLOW;  // 0xFFFF00 
	static ibColor SOLID_MAGENTA; // 0xFF00FF
	static ibColor SOLID_CYAN;    // 0x00FFFF
	static ibColor SOLID_WHITE;   // 0xFFFFFF
};

class ibColorF
{
public:
	explicit ibColorF( u32 color );
	ibColorF( u8 red, u8 green, u8 blue, u8 alpha );
	ibColorF( f32 red, f32 green, f32 blue, f32 alpha );
	explicit ibColorF( const ibColor& );
	ibColorF( const ibColorF& );

	operator float*() { return &r; }

	f32 r, g, b, a;

	static ibColorF SOLID_BLACK;   // 0x000000
	static ibColorF SOLID_RED;     // 0xFF0000
	static ibColorF SOLID_GREEN;   // 0x00FF00
	static ibColorF SOLID_BLUE;    // 0x0000FF
	static ibColorF SOLID_YELLOW;  // 0xFFFF00 
	static ibColorF SOLID_MAGENTA; // 0xFF00FF
	static ibColorF SOLID_CYAN;    // 0x00FFFF
	static ibColorF SOLID_WHITE;   // 0xFFFFFF
};

#endif // IB_COLOR_H
