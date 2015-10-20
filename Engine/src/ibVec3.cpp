#include "ibVec3.h"

#include "ibEngineDefines.h"
#include "ibMatrix.h"
#include "ibQuat.h"

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


// Operator versions exist for all binary operations except cross product
ibVec3 ibVec3::operator+ ( const ibVec3& rhs ) const
{
	return ibVec3( x + rhs.x, y + rhs.y, z + rhs.z );
}

ibVec3 ibVec3::operator- ( const ibVec3& rhs ) const
{
	return ibVec3( x - rhs.x, y - rhs.y, z - rhs.z );
}

ibVec3 ibVec3::operator* ( const float rhs ) const // v * a;
{
	return ibVec3( x * rhs, y * rhs, z * rhs );
}

float ibVec3::operator* ( const ibVec3& rhs) const // Dot product
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

ibVec3 ibVec3::operator* ( const ibMtx& rhs ) const
{
	return ibVec3( *this ) *= rhs;
}

ibVec3 ibVec3::operator* ( const ibQuat& rhs ) const
{
	return ibVec3( *this ) *= rhs;
}

// The versions with assignment are analogous to the above for the member functions
// These return a reference to this
ibVec3& ibVec3::operator+= ( const ibVec3& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

ibVec3& ibVec3::operator-= ( const ibVec3& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

ibVec3& ibVec3::operator*= ( const float rhs ) // v * a
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

ibVec3& ibVec3::operator*= ( const ibMtx& rhs )
{
	// This takes 'w' == 1
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

ibVec3& ibVec3::operator*= ( const ibQuat& rhs )
{
	f32 a2 = rhs.x * rhs.x;
	f32 b2 = rhs.y * rhs.y;
	f32 c2 = rhs.z * rhs.z;
	f32 ab = rhs.x * rhs.y;
	f32 ac = rhs.x * rhs.z;
	f32 bc = rhs.y * rhs.z;

	f32 ta = rhs.w * rhs.x;
	f32 tb = rhs.w * rhs.y;
	f32 tc = rhs.w * rhs.z;

	f32 xp = x * (0.5f - (b2 + c2)) + y * (ab - tc) + z * (ac + tb);
	f32 yp = x * (ab + tc) + y * (0.5f - (a2 + c2)) + z * (ac - ta);
	f32 zp = x * (ac - tb) + y * (bc + ta) + z * (0.5f - (a2 + b2));

	x = 2 * xp;
	y = 2 * yp;
	z = 2 * zp;

	return *this;
}

IB_EXPORT
ibVec3 operator* ( const float lhs, const ibVec3& rhs )
{
	return ibVec3( rhs.x * lhs, rhs.y * lhs, rhs.z * lhs );
}

IB_EXPORT
bool operator==( const ibVec3& lhs, const ibVec3& rhs )
{
	return (fabs(lhs.x - rhs.x) < IB_EPSILON_F ) &&
		   (fabs(lhs.y - rhs.y) < IB_EPSILON_F ) &&
		   (fabs(lhs.z - rhs.z) < IB_EPSILON_F );
}
