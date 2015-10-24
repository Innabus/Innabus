#include "ibVec2.h"

#include "ibMath.h"
#include "ibMtx2.h"

#include <cmath>

const ibVec2 ibVec2::ZERO = ibVec2( 0, 0 );
const ibVec2 ibVec2::ONE = ibVec2( 1, 1 );
const ibVec2 ibVec2::X_AXIS = ibVec2( 1, 0 );
const ibVec2 ibVec2::Y_AXIS = ibVec2( 0, 1 );

ibVec2::ibVec2( const f32 _x, const f32 _y):x(_x), y(_y) {}
ibVec2::ibVec2( const ibVec2& rhs ):x(rhs.x), y(rhs.y) {}
ibVec2& ibVec2::operator= ( const ibVec2& rhs )
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

float ibVec2::Magnitude() const
{
	return sqrt(MagnitudeS());
}

float ibVec2::MagnitudeS() const
{
	return (x * x) + (y * y);
}

ibVec2& ibVec2::Normalize()
{
	f32 mag = Magnitude();
	x /= mag;
	y /= mag;
	return *this;
}

ibVec2& ibVec2::NormalizeCopy( ibVec2& to ) const
{
	f32 mag = Magnitude();
	to.x = x / mag;
	to.y = y / mag;
	return to;
}

// Member versions, work in place.
ibVec2& ibVec2::Add( const ibVec2& rhs )
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

ibVec2& ibVec2::Sub( const ibVec2& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

ibVec2& ibVec2::Mul( const f32 rhs )
{
	x *= rhs;
	y *= rhs;
	return *this;
}

ibVec2& ibVec2::Mul( const ibMtx2& rhs )
{
	*this = ibVec2::Mul(*this, rhs);
	return *this;
}

f32 ibVec2::Dot( const ibVec2& rhs)
{
	return (x * rhs.x) + (y * rhs.y);
}

ibVec2& ibVec2::Stabelize()
{
	if (fabs(x) < IB_EPSILON_F)
		x = 0;
	if (fabs(y) < IB_EPSILON_F)
		y = 0;
	return *this;
}

// Non-meber
ibVec2 ibVec2::Add( const ibVec2& lhs, const ibVec2& rhs )
{
	return ibVec2(lhs).Add(rhs);
}

ibVec2 ibVec2::Sub( const ibVec2& lhs, const ibVec2& rhs )
{
	return ibVec2(lhs).Sub(rhs);
}

ibVec2 ibVec2::Mul( const ibVec2& lhs, const f32 rhs )
{
	return ibVec2(lhs).Mul(rhs);
}

ibVec2 ibVec2::Mul( const ibVec2& lhs, const ibMtx2& rhs )
{
	return ibVec2(
		(lhs.x * rhs.data.mtx._00) + (lhs.y * rhs.data.mtx._10),
		(lhs.x * rhs.data.mtx._01) + (lhs.y * rhs.data.mtx._11));
}

f32 ibVec2::Dot( const ibVec2& lhs, const ibVec2& rhs )
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

ibVec2 ibVec2::Stabelize( const ibVec2& v )
{
	return ibVec2(v).Stabelize();
}

// Binary operators
IB_EXPORT ibVec2 operator+ ( const ibVec2& lhs, const ibVec2& rhs )
{
	return ibVec2::Add(lhs, rhs);
}

IB_EXPORT ibVec2 operator- ( const ibVec2& lhs, const ibVec2& rhs )
{
	return ibVec2::Sub(lhs, rhs);
}

IB_EXPORT ibVec2 operator* ( const ibVec2& lhs, const f32 s )
{
	return ibVec2::Mul(lhs, s);
}

IB_EXPORT ibVec2 operator* ( const f32 s, const ibVec2& rhs )
{
	return ibVec2::Mul(rhs, s);
}

IB_EXPORT f32 operator* ( const ibVec2& lhs, const ibVec2& rhs )
{
	return ibVec2::Dot(lhs, rhs);
}

IB_EXPORT ibVec2 operator* ( const ibVec2& lhs, const ibMtx2& rhs )
{
	return ibVec2::Mul(lhs, rhs);
}

IB_EXPORT bool operator==( const ibVec2& lhs, const ibVec2& rhs )
{
	return fcmp(lhs.x, rhs.x) && fcmp(lhs.y, rhs.y);
}

IB_EXPORT bool operator!=( const ibVec2& lhs, const ibVec2& rhs )
{
	return !(lhs == rhs);
}

IB_EXPORT ibVec2& operator+= ( ibVec2& lhs, const ibVec2& rhs )
{
	return lhs.Add(rhs);
}

IB_EXPORT ibVec2& operator-= ( ibVec2& lhs, const ibVec2& rhs )
{
	return lhs.Sub(rhs);
}

IB_EXPORT ibVec2& operator*= ( ibVec2& lhs, const f32 s )
{
	return lhs.Mul(s);
}

IB_EXPORT ibVec2& operator*= ( ibVec2& lhs, const ibMtx2& rhs )
{
	return lhs.Mul(rhs);
}
