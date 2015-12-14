#include "ibColor.h"

ibColor ibColor::SOLID_BLACK   = ibColor(    0,    0,    0, 0xFF );
ibColor ibColor::SOLID_RED     = ibColor( 0xFF,    0,    0, 0xFF );
ibColor ibColor::SOLID_GREEN   = ibColor(    0, 0xFF,    0, 0xFF );
ibColor ibColor::SOLID_BLUE    = ibColor(    0,    0, 0xFF, 0xFF );
ibColor ibColor::SOLID_YELLOW  = ibColor( 0xFF, 0xFF,    0, 0xFF );
ibColor ibColor::SOLID_MAGENTA = ibColor( 0xFF,    0, 0xFF, 0xFF );
ibColor ibColor::SOLID_CYAN    = ibColor(    0, 0xFF, 0xFF, 0xFF );
ibColor ibColor::SOLID_WHITE   = ibColor( 0xFF, 0xFF, 0xFF, 0xFF );

ibColorF ibColorF::SOLID_BLACK   = ibColorF( 0.f, 0.f, 0.f, 1.f );
ibColorF ibColorF::SOLID_RED     = ibColorF( 1.f, 0.f, 0.f, 1.f );
ibColorF ibColorF::SOLID_GREEN   = ibColorF( 0.f, 1.f, 0.f, 1.f );
ibColorF ibColorF::SOLID_BLUE    = ibColorF( 0.f, 0.f, 1.f, 1.f );
ibColorF ibColorF::SOLID_YELLOW  = ibColorF( 1.f, 1.f, 0.f, 1.f );
ibColorF ibColorF::SOLID_MAGENTA = ibColorF( 1.f, 0.f, 1.f, 1.f );
ibColorF ibColorF::SOLID_CYAN    = ibColorF( 0.f, 1.f, 1.f, 1.f );
ibColorF ibColorF::SOLID_WHITE   = ibColorF( 1.f, 1.f, 1.f, 1.f );

#define COL_MIN( a, b ) (((a)<(b))?(a):(b))
#define COL_MAX( a, b ) (((a)<(b))?(b):(a))
#define COLOR_CLAMP( a, low, high ) COL_MAX( COL_MIN( (a), high ), low )

ibColor::ibColor( u32 c )
{
	// 0	xrrggbbaa
	//rgba.r = (c >> 24) & 0xFF;
	//rgba.g = (c >> 16) & 0xFF;
	//rgba.b = (c >> 8 ) & 0xFF;
	//rgba.a = c & 0xFF;
	//color = c;
	rgba.r = c & 0xFF;
	rgba.g = (c >> 8 ) & 0xFF;
	rgba.b = (c >> 16) & 0xFF;
	rgba.a = (c >> 24) & 0xFF;
}

ibColor::ibColor( u8* pBytes )
{
	ibAssert( pBytes );

	// Expects [r][g][b][a] bytes
	rgba.r = pBytes[0];
	rgba.g = pBytes[1];
	rgba.b = pBytes[2];
	rgba.a = pBytes[3];
}

ibColor::ibColor( u8 red, u8 green, u8 blue, u8 alpha )
{
	rgba.r = (red);
	rgba.g = (green);
	rgba.b = (blue);
	rgba.a = (alpha);
}

ibColor::ibColor( const ibColorF& rhs )
{
	rgba.r = (u8)(255 * COLOR_CLAMP( rhs.r, 0.f, 1.f ));
	rgba.g = (u8)(255 * COLOR_CLAMP( rhs.g, 0.f, 1.f ));
	rgba.b = (u8)(255 * COLOR_CLAMP( rhs.b, 0.f, 1.f ));
	rgba.a = (u8)(255 * COLOR_CLAMP( rhs.a, 0.f, 1.f ));
}

ibColor::ibColor( const ibColor& rhs ):
rgba( rhs.rgba )
{
}

ibColorF::ibColorF( u32 color )
{
	r = ((color >> 24) & 0xFF) / 255.f;
	g = ((color >> 16) & 0xFF) / 255.f;
	b = ((color >> 8 ) & 0xFF) / 255.f;
	a = ((color) & 0xFF) / 255.f;
}

ibColorF::ibColorF( u8 red, u8 green, u8 blue, u8 alpha ):
r( red / 255.f ),
g( green / 255.f ),
b( blue / 255.f ),
a( alpha / 255.f )
{
}

ibColorF::ibColorF( f32 red, f32 green, f32 blue, f32 alpha ):
r( red ),
g( green ),
b( blue ),
a( alpha )
{
}

ibColorF::ibColorF( const ibColor& rhs ):
r( rhs.rgba.r / 255.f ),
g( rhs.rgba.g / 255.f ),
b( rhs.rgba.b / 255.f ),
a( rhs.rgba.a / 255.f )
{
}

ibColorF::ibColorF( const ibColorF& rhs ):
r( rhs.r ),
g( rhs.g ),
b( rhs.b ),
a( rhs.a )
{
}
