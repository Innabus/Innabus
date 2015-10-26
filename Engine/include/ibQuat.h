#ifndef DN_QUAT_H
#define DN_QUAT_H
#pragma once

#include "ibDataTypes.h"
#include "ibMtx4.h"

class ibVec3;

class ibQuat
{
public:
	ibQuat();
	ibQuat( const float _w, const float _x, const float _y, const float _z ); // Direct init
	ibQuat( const float pitch, const float yaw, const float roll ); // From euler angles
	ibQuat( const float angle, const ibVec3& v ); // from vector & angle
	ibQuat( const ibQuat& rhs ); // copy
	ibQuat& operator= ( const ibQuat& rhs ); // assign

	float Magnitude() const;
	float MagnitudeS() const;
	ibQuat& Normalize();
	ibQuat& Mul( const ibQuat& rhs );
	ibQuat& Invert();
	ibQuat GetConjugate() const; // Assumes normalized
	ibQuat GetInverse() const;
	ibMtx4 GetMatrix() const;

	void ToAxisAngle( ibVec3* axis, float* angle ) const;

	ibVec3 RotatePt( ibVec3 Pt );

	static ibQuat Lerp( const ibQuat& start, const ibQuat& end, float alpha );
	static ibQuat Slerp( const ibQuat& start, const ibQuat& end, float alpha );

	ibQuat operator + ( const ibQuat & rhs ) const;
	ibQuat operator - ( const ibQuat & rhs ) const;
	ibQuat operator * ( const ibQuat & rhs ) const;
	ibQuat operator / ( const ibQuat & rhs ) const;
	ibQuat operator * ( const ibVec3 & rhs ) const;
	ibQuat operator * ( const f32 & rhs ) const;
	ibQuat operator / ( const f32 & rhs ) const;
	ibQuat operator ~ () const;

	bool operator == ( const ibQuat & rhs ) const;
	bool operator != ( const ibQuat & rhs ) const;

	ibQuat& operator += ( const ibQuat & rhs );
	ibQuat& operator -= ( const ibQuat & rhs );
	ibQuat& operator *= ( const ibQuat & rhs );
	ibQuat& operator *= ( const f32 & rhs );
	ibQuat& operator /= ( const f32 & rhs );

	float w, x, y, z;
};

#endif // DN_QUAT_H
