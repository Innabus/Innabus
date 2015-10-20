#ifndef IB_VECTOR_H
#define IB_VECTOR_H
#pragma once

class ibMtx;
class ibQuat;

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
	float Dot( const ibVec3& rhs );
	ibVec3& Cross( const ibVec3& rhs );

	// Non-member
	static ibVec3 Add( const ibVec3& lhs, const ibVec3& rhs );
	static ibVec3 Sub( const ibVec3& lhs, const ibVec3& rhs );
	static ibVec3 Mul( const float lhs, const ibVec3& rhs );
	static ibVec3 Mul( const ibVec3& lhs, const float rhs );
	static float Dot( const ibVec3& lhs, const ibVec3& rhs );
	static ibVec3 Cross( const ibVec3& lhs, const ibVec3& rhs );

	// Operator versions exist for all binary operations except cross product
	ibVec3 operator+ ( const ibVec3& rhs ) const;
	ibVec3 operator- ( const ibVec3& rhs ) const;
	ibVec3 operator* ( const float rhs ) const; // v * a;
	ibVec3 operator* ( const ibMtx& rhs ) const;
	ibVec3 operator* ( const ibQuat& rhs ) const;

	float operator* ( const ibVec3& rhs) const; // Dot product

	// The versions with assignment are analogous to the above for the member functions
	// These return a reference to this
	ibVec3& operator+= ( const ibVec3& rhs );
	ibVec3& operator-= ( const ibVec3& rhs );
	ibVec3& operator*= ( const float rhs ); // v * a
	ibVec3& operator*= ( const ibMtx& rhs );
	ibVec3& operator*= ( const ibQuat& rhs );

	// Data
	float x, y, z; // w = 0/1 depending on usage; actually I think w = 0

	static const ibVec3 ZERO; // { 0, 0, 0 }
	static const ibVec3 ONE; // { 1, 1, 1 }
	static const ibVec3 X_AXIS; // { 1, 0, 0 }
	static const ibVec3 Y_AXIS; // { 0, 1, 0 }
	static const ibVec3 Z_AXIS; // { 0, 0, 1 }
};

IB_EXPORT ibVec3 operator* ( const float lhs, const ibVec3& rhs );
IB_EXPORT bool operator==( const ibVec3& lhs, const ibVec3& rhs );

#endif // IB_VECTOR_H
