#include "ibVec4.h"

#include "ibMtx4.h"

#include <math.h>

const ibVec4 ibVec4::ZERO = ibVec4( 0, 0, 0, 0 );
const ibVec4 ibVec4::ONE = ibVec4( 1, 1, 1, 1 );
const ibVec4 ibVec4::X_AXIS = ibVec4( 1, 0, 0, 0 );
const ibVec4 ibVec4::Y_AXIS = ibVec4( 0, 1, 0, 0 );
const ibVec4 ibVec4::Z_AXIS = ibVec4( 0, 0, 1, 0 );
const ibVec4 ibVec4::W_AXIS = ibVec4( 0, 0, 0, 1 );

ibVec4::ibVec4( const f32 _x, const f32 _y, const f32 _z, const f32 _w ):
x(_x), y(_y), z(_z), w(_w)
{
}

ibVec4::ibVec4( const ibVec4& rhs ):
x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
}

ibVec4& ibVec4::operator= ( const ibVec4& rhs )
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}


f32 ibVec4::Magnitude() const
{
	return sqrt( MagnitudeS() );
}

f32 ibVec4::MagnitudeS() const // Like magnitude, but does not sqrt (equivalent to A.Dot(A))
{
	return x*x + y*y + z*z + w*w;
}

ibVec4& ibVec4::Normalize()
{
	if ( f32 m = Magnitude() )
	{
		x /= m;
		y /= m;
		z /= m;
		w /= m;
	}
	return *this;
}

ibVec4& ibVec4::NormalizeCopy( ibVec4& to ) const
{
	if ( f32 m = Magnitude() )
	{
		to.x = x / m;
		to.y = y / m;
		to.z = z / m;
		to.w = w / m;
	}
	return to;
}


// Member versions work "in place"
ibVec4& ibVec4::Add( const ibVec4& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

ibVec4& ibVec4::Sub( const ibVec4& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

ibVec4& ibVec4::Mul( const f32 rhs )
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

ibVec4& ibVec4::Mul( const ibMtx4& rhs)
{
	f32 xp = (x * rhs.data.mtx._00) 
		     + (y * rhs.data.mtx._10)
			 + (z * rhs.data.mtx._20)
			 + (w * rhs.data.mtx._30);

	f32 yp = (x * rhs.data.mtx._01) 
		     + (y * rhs.data.mtx._11)
			 + (z * rhs.data.mtx._21)
			 + (w * rhs.data.mtx._31);

	f32 zp = (x * rhs.data.mtx._02) 
		     + (y * rhs.data.mtx._12)
			 + (z * rhs.data.mtx._22)
			 + (w * rhs.data.mtx._32);

	f32 wp = (x * rhs.data.mtx._03)
		     + (y * rhs.data.mtx._13)
			 + (z * rhs.data.mtx._23)
			 + (w * rhs.data.mtx._33);

	x = xp;
	y = yp;
	z = zp;
	w = wp;
	return *this;
}

f32 ibVec4::Dot( const ibVec4& rhs ) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

ibVec4& ibVec4::Stabelize()
{
	if (fabs(x) < IB_EPSILON_F)
		x = 0;
	if (fabs(y) < IB_EPSILON_F)
		y = 0;
	if (fabs(z) < IB_EPSILON_F)
		z = 0;
	if (fabs(w) < IB_EPSILON_F)
		w = 0;
	return *this;
}

// Non-member
ibVec4 ibVec4::Add( const ibVec4& lhs, const ibVec4& rhs )
{
	return ibVec4(lhs).Add(rhs);
}

ibVec4 ibVec4::Sub( const ibVec4& lhs, const ibVec4& rhs )
{
	return ibVec4(lhs).Sub(rhs);
}

ibVec4 ibVec4::Mul( const f32 lhs, const ibVec4& rhs )
{
	return ibVec4(rhs).Mul(lhs);
}

ibVec4 ibVec4::Mul( const ibVec4& lhs, const f32 rhs )
{
	return ibVec4(lhs).Mul(rhs);
}

ibVec4 ibVec4::Mul( const ibVec4& lhs, const ibMtx4& rhs )
{
	return ibVec4(lhs).Mul(rhs);
}

f32 ibVec4::Dot( const ibVec4& lhs, const ibVec4& rhs )
{
	return lhs.Dot(rhs);
}

ibVec4 ibVec4::Stabelize( const ibVec4& a )
{
	return ibVec4(a).Stabelize();
}
