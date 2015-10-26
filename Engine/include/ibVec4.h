#ifndef IB_VEC4_H
#define IB_VEC4_H
#pragma once

#include "ibMath.h"

class ibMtx4;

class IB_EXPORT ibVec4
{
public:
	ibVec4() {}; // No default init!
	ibVec4( const f32 _x, const f32 _y, const f32 _z, const f32 _w );
	ibVec4( const ibVec4& rhs );
	ibVec4& operator= ( const ibVec4& rhs );

	f32 Magnitude() const;
	f32 MagnitudeS() const; // Like magnitude, but does not sqrt

	ibVec4& Normalize();
	ibVec4& NormalizeCopy( ibVec4& to ) const;

	// Member versions work "in place"
	ibVec4& Add( const ibVec4& rhs );
	ibVec4& Sub( const ibVec4& rhs );
	ibVec4& Mul( const f32 rhs );
	ibVec4& Mul( const ibMtx4& rhs);
	f32 Dot( const ibVec4& rhs ) const;

	ibVec4& Stabelize();

	// Non-member
	static ibVec4 Add( const ibVec4& lhs, const ibVec4& rhs );
	static ibVec4 Sub( const ibVec4& lhs, const ibVec4& rhs );
	static ibVec4 Mul( const f32 lhs, const ibVec4& rhs );
	static ibVec4 Mul( const ibVec4& lhs, const f32 rhs );
	static ibVec4 Mul( const ibVec4& lhs, const ibMtx4& rhs );
	static f32 Dot( const ibVec4& lhs, const ibVec4& rhs );

	static ibVec4 Stabelize( const ibVec4& v );

	// Data
	f32 x, y, z, w;

	static const ibVec4 ZERO; // { 0, 0, 0, 0 }
	static const ibVec4 ONE; // { 1, 1, 1, 1 }
	static const ibVec4 X_AXIS; // { 1, 0, 0, 0 }
	static const ibVec4 Y_AXIS; // { 0, 1, 0, 0 }
	static const ibVec4 Z_AXIS; // { 0, 0, 1, 0 }
	static const ibVec4 W_AXIS; // { 0, 0, 0, 1 }
};

inline ibVec4 operator+ ( const ibVec4& lhs, const ibVec4& rhs ) { return ibVec4::Add(lhs, rhs); }
inline ibVec4 operator- ( const ibVec4& lhs, const ibVec4& rhs ) { return ibVec4::Sub(lhs, rhs); }
inline ibVec4 operator* ( const ibVec4& lhs, const f32 rhs ) { return ibVec4::Mul(lhs, rhs); }
inline ibVec4 operator* ( const f32 lhs, const ibVec4& rhs ) { return ibVec4::Mul(rhs, lhs); }
inline ibVec4 operator* ( const ibVec4& lhs, const ibMtx4& rhs ) { return ibVec4::Mul(lhs, rhs); }
inline f32 operator* ( const ibVec4& lhs, const ibVec4& rhs ) { return ibVec4::Dot(lhs, rhs); }

inline ibVec4& operator+= ( ibVec4& lhs, const ibVec4& rhs ) { return lhs.Add(rhs); }
inline ibVec4& operator-= ( ibVec4& lhs, const ibVec4& rhs ) { return lhs.Sub(rhs); }
inline ibVec4& operator*= ( ibVec4& lhs, const f32 rhs ) { return lhs.Mul(rhs); }
inline ibVec4& operator*= ( ibVec4& lhs, const ibMtx4& rhs ) { return lhs.Mul(rhs); }

inline bool operator==( const ibVec4& lhs, const ibVec4& rhs ) { 	
	return fcmp(lhs.x, rhs.x) && fcmp(lhs.y, rhs.y) && fcmp(lhs.z, rhs.z);
}
inline bool operator!=( const ibVec4& lhs, const ibVec4& rhs ) { return !(lhs == rhs); }

#endif // IB_VEC4_H
