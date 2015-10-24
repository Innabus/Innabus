#ifndef IB_VEC2_H
#define IB_VEC2_H
#pragma once

class ibMtx2;

class IB_EXPORT ibVec2
{
public:
	ibVec2() {}; // No default init!
	ibVec2( const f32 _x, const f32 _y);
	ibVec2( const ibVec2& rhs );
	ibVec2& operator= ( const ibVec2& rhs );

	float Magnitude() const;
	float MagnitudeS() const; // Magnitude squared

	ibVec2& Normalize();
	ibVec2& NormalizeCopy( ibVec2& to ) const;

	// Member versions, work in place.
	ibVec2& Add( const ibVec2& rhs );
	ibVec2& Sub( const ibVec2& rhs );
	ibVec2& Mul( const f32 rhs );
	ibVec2& Mul( const ibMtx2& rhs);
	f32 Dot( const ibVec2& rhs);

	ibVec2& Stabelize();

	// Non-member
	static ibVec2 Add( const ibVec2& lhs, const ibVec2& rhs );
	static ibVec2 Sub( const ibVec2& lhs, const ibVec2& rhs );
	static ibVec2 Mul( const ibVec2& lhs, const f32 rhs );
	static ibVec2 Mul( const ibVec2& lhs, const ibMtx2& rhs);
	static f32 Dot( const ibVec2& lhs, const ibVec2& rhs );

	static ibVec2 Stabelize( const ibVec2& v );

	// Data
	float x, y;

	static const ibVec2 ZERO;
	static const ibVec2 ONE;
	static const ibVec2 X_AXIS;
	static const ibVec2 Y_AXIS;
};

// Binary operators
IB_EXPORT ibVec2 operator+ ( const ibVec2& lhs, const ibVec2& rhs );
IB_EXPORT ibVec2 operator- ( const ibVec2& lhs, const ibVec2& rhs );
IB_EXPORT ibVec2 operator* ( const ibVec2& lhs, const f32 s );
IB_EXPORT ibVec2 operator* ( const f32 s, const ibVec2& rhs );
IB_EXPORT f32 operator* ( const ibVec2& lhs, const ibVec2& rhs );
IB_EXPORT ibVec2 operator* ( const ibVec2& lhs, const ibMtx2& rhs );

IB_EXPORT ibVec2& operator+= ( ibVec2& lhs, const ibVec2& rhs );
IB_EXPORT ibVec2& operator-= ( ibVec2& lhs, const ibVec2& rhs );
IB_EXPORT ibVec2& operator*= ( ibVec2& lhs, const f32 s );
IB_EXPORT ibVec2& operator*= ( ibVec2& lhs, const ibMtx2& rhs );

IB_EXPORT bool operator==( const ibVec2& lhs, const ibVec2& rhs );
IB_EXPORT bool operator!=( const ibVec2& lhs, const ibVec2& rhs );

#endif // IB_VEC2_H
