#include "ibQuat.h"
#include "ibVec3.h"

#include "ibMath.h"

#include <math.h>

ibQuat::ibQuat():w(1.f), x(0.f), y(0.f), z(0.f)
{
}

ibQuat::ibQuat( float _w, float _x, float _y, float _z ):
w(_w), x(_x), y(_y), z(_z)
{
}

ibQuat::ibQuat( const float pitch, const float yaw, const float roll )
{
	const float PIOVER360 = ibMath::Pi / 360.f;
	const float p = pitch * PIOVER360;
	const float _y = yaw * PIOVER360;
	const float r = roll * PIOVER360;

	float sinp = sin(p), cosp = cos(p);
	float siny = sin(_y), cosy = cos(_y);
	float sinr = sin(r), cosr = cos(r);

	w = cosr * cosp * cosy + sinr * sinp * siny;
	x = sinr * cosp * cosy - cosr * sinp * siny;
	y = cosr * sinp * cosy + sinr * cosp * siny;
	z = cosr * cosp * siny - sinr * sinp * cosy;

	Normalize();
}

ibQuat::ibQuat( const float angle, const ibVec3& v )
{
	w = cos( angle / 2 );
	ibVec3 copy(v);
	copy.Normalize();
	f32 sin_a2 = sin( angle/2 );
	x = copy.x * sin_a2;
	y = copy.y * sin_a2;
	z = copy.z * sin_a2;
}

ibQuat::ibQuat( const ibQuat& rhs ):
w(rhs.w), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

ibQuat& ibQuat::operator= ( const ibQuat& rhs )
{
	w = rhs.w;
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

float ibQuat::Magnitude() const
{
	return sqrt( w*w + x*x + y*y + z*z );
}

float ibQuat::MagnitudeS() const
{
	return w*w + x*x + y*y + z*z;
}

ibQuat& ibQuat::Normalize()
{
	float m = Magnitude();
	if ( fabs(m) - 1.0f )
	{
		w /= m;
		x /= m;
		y /= m;
		z /= m;
	}
	return *this;
}

ibQuat& ibQuat::Mul( const ibQuat& rhs )
{
	float tmps[4];
	tmps[0] = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	tmps[1] = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
	tmps[2] = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
	tmps[3] = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
	w = tmps[0];
	x = tmps[1];
	y = tmps[2];
	z = tmps[3];
	return *this;
}

ibQuat& ibQuat::Invert()
{
	float length = 1.f / (x*x + y*y + z*z + w*w);
	w *= length;
	x *= -length;
	y *= -length;
	z *= -length;
	return *this;
}

ibQuat ibQuat::GetConjugate() const
{
	return ibQuat( w, -x, -y, -z );
}

ibQuat ibQuat::GetInverse() const
{
	float length = 1.f / (x*x + y*y + z*z + w*w);
	return ibQuat( w * length, x * -length, y * -length, z * -length );
}

ibMtx4 ibQuat::GetMatrix() const
{
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;
 
	// This calculation would be a lot more complicated for non-unit length quaternions
	return ibMtx4( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy + wz),        2.0f * (xz - wy),        0.0f,
			       2.0f * (xy - wz),        1.0f - 2.0f * (x2 + z2), 2.0f * (yz + wx),        0.0f,
			       2.0f * (xz + wy),        2.0f * (yz - wx),        1.0f - 2.0f * (x2 + y2), 0.0f,
			       0.0f,                    0.0f,                    0.0f,                    1.0f);
}

void ibQuat::ToAxisAngle( ibVec3* axis, float* angle ) const
{
	ibQuat qNorm( *this );
	qNorm.Normalize();
	
	float sinAngle = sqrt(1.0f - (qNorm.w*qNorm.w));
	if (fabs(sinAngle) < 0.0005f)
		sinAngle = 1.0f;

	if ( axis )
	{
		axis->x = qNorm.x / sinAngle;
		axis->y = qNorm.y / sinAngle;
		axis->z = qNorm.z / sinAngle;
	}

	if ( angle )
	{
		*angle = 2.f * acos( qNorm.w );
	}
}

ibVec3 ibQuat::RotatePt( ibVec3 p )
{
	// qp(q^-1)

	// First calculate qp = [s, A][0, B]
	// == [-A.B, sB + A x B]
	f32 AdotB = (x * p.x) + (y * p.y) + (z * p.z);

	// Find sB + A x B
	ibVec3 sBplusAcrossB( 
		(y * p.z) - (z * p.y) + (w * p.x),
		(z * p.x) - (x * p.z) + (w * p.y),
		(x * p.y) - (y * p.x) + (w * p.z)
	);

	ibQuat qp( AdotB, sBplusAcrossB.x, sBplusAcrossB.y, sBplusAcrossB.z );
	qp.Mul( GetConjugate() );

	return ibVec3( qp.x, qp.y, qp.z );
}

/* static */
ibQuat ibQuat::Lerp( const ibQuat& start, const ibQuat& end, float alpha )
{
	return ibQuat( start * (1.0f - alpha) + end * alpha );
}

#define SLERP_TO_LERP_SWITCH_THRESHOLD 0.01f

/* static */
ibQuat ibQuat::Slerp( const ibQuat& start, const ibQuat& end, float alpha )
{
	f32 angle = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
	f32 sign = 1.0f;
	if (angle < 0.0f)
	{
		angle = -angle;
		sign = -1.0f;
	}
	
	f32 startWeight = 1.f - alpha;
	f32 endWeight = alpha;

	if (angle <= 0.97f)
	{
		angle = acosf(angle);
		f32 oneOverSinTheta  = 1.0f / sinf(angle);
		startWeight = sinf((1.0f - alpha) * angle) * oneOverSinTheta;
		endWeight = sinf(angle * alpha) * oneOverSinTheta;
	}

	startWeight *= sign;

	ibQuat result(
		(start.w * startWeight) + (end.w * endWeight),
		(start.x * startWeight) + (end.x * endWeight),
		(start.y * startWeight) + (end.y * endWeight),
		(start.z * startWeight) + (end.z * endWeight)
	);

	return result.Normalize();
}

ibQuat ibQuat::operator + ( const ibQuat & rhs ) const
{
	return ibQuat( w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z );
}

ibQuat ibQuat::operator - ( const ibQuat & rhs ) const
{
	return ibQuat( w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z );
}

ibQuat ibQuat::operator * ( const ibQuat & rhs ) const
{
	return ibQuat( *this ) *= rhs;
}

ibQuat ibQuat::operator / ( const ibQuat & rhs ) const
{
	return (*this) * ~rhs;
}

ibQuat ibQuat::operator * ( const ibVec3 & rhs ) const
{
	ibQuat tmp( 0.f, rhs.x, rhs.y, rhs.z );
	return ibQuat( *this ) *= tmp;
}

ibQuat ibQuat::operator * ( const f32 & rhs ) const
{
	return ibQuat( w * rhs, x * rhs, y * rhs, z * rhs );
}

ibQuat ibQuat::operator / ( const f32 & rhs ) const
{
	return ibQuat( w / rhs, x / rhs, y / rhs, z / rhs );
}

ibQuat ibQuat::operator~() const
{
	return GetConjugate();
}

bool ibQuat::operator == ( const ibQuat & rhs ) const
{
	return ((w == rhs.w) && (x == rhs.x) && (y == rhs.y) && (z == rhs.z));
}

bool ibQuat::operator != ( const ibQuat & rhs ) const
{
	return !(*this == rhs);
}

ibQuat& ibQuat::operator += ( const ibQuat & rhs )
{
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

ibQuat& ibQuat::operator -= ( const ibQuat & rhs )
{
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

ibQuat& ibQuat::operator *= ( const ibQuat & rhs )
{
	return Mul( rhs );
}

ibQuat& ibQuat::operator *= ( const f32 & rhs )
{
	w *= rhs;
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

ibQuat& ibQuat::operator /= ( const f32 & rhs )
{
	w /= rhs;
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}
