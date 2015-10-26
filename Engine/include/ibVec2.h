#ifndef IB_VEC2_H
#define IB_VEC2_H
#pragma once

#include "ibMath.h"

class ibMtx2;

class IB_EXPORT ibVec2
{
public:
	ibVec2() {}; // No default init!
	ibVec2( const f32 _x, const f32 _y);
	ibVec2( const ibVec2& rhs );
	ibVec2& operator= ( const ibVec2& rhs );

	f32 Magnitude() const;
	f32 MagnitudeS() const; // Magnitude squared

	ibVec2& Normalize();
	ibVec2& NormalizeCopy( ibVec2& to ) const;

	// Member versions, work in place.
	ibVec2& Add( const ibVec2& rhs );
	ibVec2& Sub( const ibVec2& rhs );
	ibVec2& Mul( const f32 rhs );
	ibVec2& Mul( const ibMtx2& rhs);
	f32 Dot( const ibVec2& rhs) const;

	ibVec2& Stabelize();

	// Non-member
	static ibVec2 Add( const ibVec2& lhs, const ibVec2& rhs );
	static ibVec2 Sub( const ibVec2& lhs, const ibVec2& rhs );
	static ibVec2 Mul( const ibVec2& lhs, const f32 rhs );
	static ibVec2 Mul( const ibVec2& lhs, const ibMtx2& rhs);
	static f32 Dot( const ibVec2& lhs, const ibVec2& rhs );

	static ibVec2 Stabelize( const ibVec2& v );

	// Data
	f32 x, y;

	static const ibVec2 ZERO;
	static const ibVec2 ONE;
	static const ibVec2 X_AXIS;
	static const ibVec2 Y_AXIS;
};

// Binary operators
inline ibVec2 operator+ ( const ibVec2& lhs, const ibVec2& rhs ) { return ibVec2::Add(lhs, rhs); }
inline ibVec2 operator- ( const ibVec2& lhs, const ibVec2& rhs ) { return ibVec2::Sub(lhs, rhs); }
inline ibVec2 operator* ( const ibVec2& lhs, const f32 s ) { return ibVec2::Mul(lhs, s); }
inline ibVec2 operator* ( const f32 s, const ibVec2& rhs ) { return ibVec2::Mul(rhs, s); }
inline f32 operator* ( const ibVec2& lhs, const ibVec2& rhs ) { return ibVec2::Dot(lhs, rhs); }
inline ibVec2 operator* ( const ibVec2& lhs, const ibMtx2& rhs ) { return ibVec2::Mul(lhs, rhs); }

inline ibVec2& operator+= ( ibVec2& lhs, const ibVec2& rhs ) { return lhs.Add(rhs); }
inline ibVec2& operator-= ( ibVec2& lhs, const ibVec2& rhs ) { return lhs.Sub(rhs); }
inline ibVec2& operator*= ( ibVec2& lhs, const f32 s ) { return lhs.Mul(s); }
inline ibVec2& operator*= ( ibVec2& lhs, const ibMtx2& rhs ) { return lhs.Mul(rhs); }

inline bool operator==( const ibVec2& lhs, const ibVec2& rhs ) { return fcmp(lhs.x, rhs.x) && fcmp(lhs.y, rhs.y); }
inline bool operator!=( const ibVec2& lhs, const ibVec2& rhs ) { return !(lhs == rhs); }

#endif // IB_VEC2_H
