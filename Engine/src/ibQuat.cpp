#include "ibQuat.h"
#include "ibVec3.h"
#include "ibVec4.h"

#include "ibMath.h"

#include <math.h>

ibQuat::ibQuat():w(1.f), x(0.f), y(0.f), z(0.f)
{
}

ibQuat::ibQuat( float _w, float _x, float _y, float _z ):
w(_w), x(_x), y(_y), z(_z)
{
}

ibQuat::ibQuat( const float yaw, const float pitch, const float roll )
{
	//const float PIOVER360 = ibMath::Pi / 360.f;
	//const float p = pitch * PIOVER360;
	//const float _y = yaw * PIOVER360;
	//const float r = roll * PIOVER360;

	//float sinp = sin(p), cosp = cos(p);
	//float siny = sin(_y), cosy = cos(_y);
	//float sinr = sin(r), cosr = cos(r);
	float sinp = sin(pitch /2), cosp = cos(pitch / 2);
	float siny = sin(yaw / 2), cosy = cos(yaw / 2);
	float sinr = sin(roll / 2), cosr = cos(roll / 2);
	
	w = cosr * cosp * cosy + sinr * sinp * siny;
	x = cosr * sinp * cosy + sinr * cosp * siny;
	y = cosr * cosp * siny - sinr * sinp * cosy;
	z = sinr * cosp * cosy - cosr * sinp * siny;

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

ibQuat& ibQuat::Add( const ibQuat& rhs )
{
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

ibQuat& ibQuat::Sub( const ibQuat& rhs )
{
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

ibQuat& ibQuat::Mul( const ibQuat& rhs )
{
	float tmps[4];
	tmps[0] = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
	tmps[1] = w * rhs.x + x * rhs.w - y * rhs.z + z * rhs.y;
	tmps[2] = w * rhs.y + x * rhs.z + y * rhs.w - z * rhs.x;
	tmps[3] = w * rhs.z - x * rhs.y + y * rhs.x + z * rhs.w;
	w = tmps[0];
	x = tmps[1];
	y = tmps[2];
	z = tmps[3];
	return *this;
}

ibQuat& ibQuat::Mul( const f32 scalar )
{
	w *= scalar;
	x *= scalar;
	y *= scalar;
	z *= scalar;
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

ibQuat ibQuat::Conjugate() const
{
	return ibQuat( w, -x, -y, -z );
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

ibVec3 ibQuat::RotatePt( const ibVec3& p ) const
{
	ibQuat res = Conjugate();
	(res *= ibQuat(0, p.x, p.y, p.z)) *= *this;
	ibVec3 v(res.x, res.y, res.z);
	return v;
}

ibVec4 ibQuat::RotatePt( const ibVec4& p ) const
{
	ibQuat res = Conjugate();
	(res *= ibQuat(0, p.x, p.y, p.z)) *= *this;
	ibVec4 v(res.x, res.y, res.z, p.w);
	return v;
}

void ibQuat::TransformPoints( ibVec3* points, u32 count ) const
{
	for (u32 n = 0; n < count; ++n)
		RotatePt(points[n]);
}

void ibQuat::TransformPoints( ibVec4* points, u32 count ) const
{
	for (u32 n = 0; n < count; ++n)
		RotatePt(points[n]);
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

/*
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
*/