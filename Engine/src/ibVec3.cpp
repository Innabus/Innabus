#include "ibVec3.h"

#include "ibMtx3.h"

#include <math.h>

const ibVec3 ibVec3::ZERO = ibVec3( 0, 0, 0 );
const ibVec3 ibVec3::ONE = ibVec3( 1, 1, 1 );
const ibVec3 ibVec3::X_AXIS = ibVec3( 1, 0, 0 );
const ibVec3 ibVec3::Y_AXIS = ibVec3( 0, 1, 0 );
const ibVec3 ibVec3::Z_AXIS = ibVec3( 0, 0, 1 );

ibVec3::ibVec3( const float _x, const float _y, const float _z ):
x(_x), y(_y), z(_z)
{
}

ibVec3::ibVec3( const ibVec3& rhs ):
x(rhs.x), y(rhs.y), z(rhs.z)
{
}

ibVec3& ibVec3::operator= ( const ibVec3& rhs )
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}


float ibVec3::Magnitude() const
{
	return sqrt( MagnitudeS() );
}

float ibVec3::MagnitudeS() const // Like magnitude, but does not sqrt (equivalent to A.Dot(A))
{
	return x*x + y*y + z*z;
}

ibVec3& ibVec3::Normalize()
{
	if ( float m = Magnitude() )
	{
		x /= m;
		y /= m;
		z /= m;
	}
	return *this;
}

ibVec3& ibVec3::NormalizeCopy( ibVec3& to ) const
{
	if ( float m = Magnitude() )
	{
		to.x = x / m;
		to.y = y / m;
		to.z = z / m;
	}
	return to;
}


// Member versions work "in place"
ibVec3& ibVec3::Add( const ibVec3& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

ibVec3& ibVec3::Sub( const ibVec3& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

ibVec3& ibVec3::Mul( const float rhs )
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

ibVec3& ibVec3::Mul( const ibMtx3& rhs)
{
	float xp = (x * rhs.data.mtx._00) 
		     + (y * rhs.data.mtx._10)
			 + (z * rhs.data.mtx._20);

	float yp = (x * rhs.data.mtx._01) 
		     + (y * rhs.data.mtx._11)
			 + (z * rhs.data.mtx._21);

	float zp = (x * rhs.data.mtx._02) 
		     + (y * rhs.data.mtx._12)
			 + (z * rhs.data.mtx._22);

	x = xp;
	y = yp;
	z = zp;
	return *this;
}

float ibVec3::Dot( const ibVec3& rhs )
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

ibVec3& ibVec3::Cross( const ibVec3& rhs )
{
	float x_ = y * rhs.z - z * rhs.y;
	float y_ = z * rhs.x - x * rhs.z;
	float z_ = x * rhs.y - y * rhs.x;
	x = x_;
	y = y_;
	z = z_;
	return *this;
}

ibVec3& ibVec3::Stabelize()
{
	if (fabs(x) < IB_EPSILON_F)
		x = 0;
	if (fabs(y) < IB_EPSILON_F)
		y = 0;
	if (fabs(z) < IB_EPSILON_F)
		z = 0;
	return *this;
}

// Non-member
ibVec3 ibVec3::Add( const ibVec3& lhs, const ibVec3& rhs )
{
	return ibVec3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

ibVec3 ibVec3::Sub( const ibVec3& lhs, const ibVec3& rhs )
{
	return ibVec3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

ibVec3 ibVec3::Mul( const float lhs, const ibVec3& rhs )
{
	return ibVec3( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z );
}

ibVec3 ibVec3::Mul( const ibVec3& lhs, const float rhs )
{
	return ibVec3( rhs * lhs.x, rhs * lhs.y, rhs * lhs.z );
}

ibVec3 ibVec3::Mul( const ibVec3& lhs, const ibMtx3& rhs )
{
	return ibVec3(lhs).Mul(rhs);
}

float ibVec3::Dot( const ibVec3& lhs, const ibVec3& rhs )
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

ibVec3 ibVec3::Cross( const ibVec3& lhs, const ibVec3& rhs )
{
	return ibVec3(
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	);
}

ibVec3 ibVec3::Stabelize( const ibVec3& a )
{
	return ibVec3(a).Stabelize();
}
