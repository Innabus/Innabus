#ifndef IB_VECTOR_H
#define IB_VECTOR_H
#pragma once

#include "ibMath.h"

class ibMtx3;

class IB_EXPORT ibVec3
{
public:
	ibVec3() {}; // No default init!
	ibVec3( const float _x, const float _y, const float _z );
	ibVec3( const ibVec3& rhs );
	ibVec3& operator= ( const ibVec3& rhs );

	float Magnitude() const;
	float MagnitudeS() const; // Like magnitude, but does not sqrt

	ibVec3& Normalize();
	ibVec3& NormalizeCopy( ibVec3& to ) const;

	// Member versions work "in place"
	ibVec3& Add( const ibVec3& rhs );
	ibVec3& Sub( const ibVec3& rhs );
	ibVec3& Mul( const float rhs );
	ibVec3& Mul( const ibMtx3& rhs);
	float Dot( const ibVec3& rhs );
	ibVec3& Cross( const ibVec3& rhs );

	ibVec3& Stabelize();

	// Non-member
	static ibVec3 Add( const ibVec3& lhs, const ibVec3& rhs );
	static ibVec3 Sub( const ibVec3& lhs, const ibVec3& rhs );
	static ibVec3 Mul( const float lhs, const ibVec3& rhs );
	static ibVec3 Mul( const ibVec3& lhs, const float rhs );
	static ibVec3 Mul( const ibVec3& lhs, const ibMtx3& rhs );
	static float Dot( const ibVec3& lhs, const ibVec3& rhs );
	static ibVec3 Cross( const ibVec3& lhs, const ibVec3& rhs );

	static ibVec3 Stabelize( const ibVec3& v );

	// Data
	float x, y, z;

	static const ibVec3 ZERO; // { 0, 0, 0 }
	static const ibVec3 ONE; // { 1, 1, 1 }
	static const ibVec3 X_AXIS; // { 1, 0, 0 }
	static const ibVec3 Y_AXIS; // { 0, 1, 0 }
	static const ibVec3 Z_AXIS; // { 0, 0, 1 }
};

inline ibVec3 operator+ ( const ibVec3& lhs, const ibVec3& rhs ) { return ibVec3::Add(lhs, rhs); }
inline ibVec3 operator- ( const ibVec3& lhs, const ibVec3& rhs ) { return ibVec3::Sub(lhs, rhs); }
inline ibVec3 operator* ( const ibVec3& lhs, const f32 rhs ) { return ibVec3::Mul(lhs, rhs); }
inline ibVec3 operator* ( const f32 lhs, const ibVec3& rhs ) { return ibVec3::Mul(rhs, lhs); }
inline ibVec3 operator* ( const ibVec3& lhs, const ibMtx3& rhs ) { return ibVec3::Mul(lhs, rhs); }
inline f32 operator* ( const ibVec3& lhs, const ibVec3& rhs ) { return ibVec3::Dot(lhs, rhs); }

inline ibVec3& operator+= ( ibVec3& lhs, const ibVec3& rhs ) { return lhs.Add(rhs); }
inline ibVec3& operator-= ( ibVec3& lhs, const ibVec3& rhs ) { return lhs.Sub(rhs); }
inline ibVec3& operator*= ( ibVec3& lhs, const f32 rhs ) { return lhs.Mul(rhs); }
inline ibVec3& operator*= ( ibVec3& lhs, const ibMtx3& rhs ) { return lhs.Mul(rhs); }

inline bool operator==( const ibVec3& lhs, const ibVec3& rhs ) { 	
	return (fabs(lhs.x - rhs.x) < IB_EPSILON_F ) &&
		   (fabs(lhs.y - rhs.y) < IB_EPSILON_F ) &&
		   (fabs(lhs.z - rhs.z) < IB_EPSILON_F );
}
inline bool operator!=( const ibVec3& lhs, const ibVec3& rhs ) { return !(lhs == rhs); }

#endif // IB_VECTOR_H
