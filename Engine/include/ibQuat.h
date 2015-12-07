#ifndef DN_QUAT_H
#define DN_QUAT_H
#pragma once

#include "ibDataTypes.h"
#include "ibMath.h"
#include "ibMtx4.h"
#include "ibVec3.h"
#include "ibVec4.h"

class IB_CORE_EXPORT ibQuat
{
public:
	ibQuat();
	ibQuat( const float _w, const float _x, const float _y, const float _z ); // Direct init
	ibQuat( const float yaw, const float pitch, const float roll ); // From euler angles
	ibQuat( const float angle, const ibVec3& v ); // from vector & angle
	ibQuat( const ibQuat& rhs ); // copy
	ibQuat& operator= ( const ibQuat& rhs ); // assign

	float Magnitude() const;
	float MagnitudeS() const;
	ibQuat& Normalize();
	ibQuat& Add( const ibQuat& rhs );
	ibQuat& Sub( const ibQuat& rhs );
	ibQuat& Mul( const ibQuat& rhs );
	ibQuat& Mul( const f32 scalar );
	ibQuat& Invert();
	ibQuat Conjugate() const; // Assumes normalized
	ibVec3 RotatePt( const ibVec3& Pt ) const;
	ibVec4 RotatePt( const ibVec4& Pt ) const;
	void TransformPoints( ibVec3* points, u32 count ) const;
	void TransformPoints( ibVec4* points, u32 count ) const;

	ibMtx4 GetMatrix() const;
	void ToAxisAngle( ibVec3* axis, float* angle ) const;

	static float Magnitude( const ibQuat& q ) { return q.Magnitude(); }
	static float MagnitudeS( const ibQuat& q ) { return q.MagnitudeS(); }
	static ibQuat Normalize( const ibQuat& q ) { return ibQuat(q).Normalize(); }
	static ibQuat Add( const ibQuat& lhs, const ibQuat& rhs ) { return ibQuat(lhs).Add(rhs); }
	static ibQuat Sub( const ibQuat& lhs, const ibQuat& rhs ) { return ibQuat(lhs).Sub(rhs); }
	static ibQuat Mul( const ibQuat& lhs, const ibQuat& rhs ) { return ibQuat(lhs).Mul(rhs); }
	static ibQuat Mul( const ibQuat& lhs, const f32 scalar ) { return ibQuat(lhs).Mul(scalar); }
	static ibQuat Conjugate( const ibQuat& lhs ) { return lhs.Conjugate(); }
	static ibQuat Invert( const ibQuat& lhs ) { return ibQuat(lhs).Invert(); }
	static ibVec3 RotatePt( const ibQuat& lhs, const ibVec3& pt ) { return lhs.RotatePt(pt); }
	static ibVec4 RotatePt( const ibQuat& lhs, const ibVec4& pt ) { return lhs.RotatePt(pt); }
	static void TransformPoints( const ibQuat& lhs, ibVec3* points, u32 count ) { lhs.TransformPoints(points, count); }
	static void TransformPoints( const ibQuat& lhs, ibVec4* points, u32 count ) { lhs.TransformPoints(points, count); }

	static ibMtx4 GetMatrix( const ibQuat& lhs ) { return lhs.GetMatrix(); }
	static void ToAxisAngle( const ibQuat& q, ibVec3* axis, float* angle ) { return q.ToAxisAngle(axis, angle); }
	
	static ibQuat Lerp( const ibQuat& start, const ibQuat& end, float alpha );
	static ibQuat Slerp( const ibQuat& start, const ibQuat& end, float alpha );

	float w, x, y, z;
};

inline ibQuat operator + ( const ibQuat& lhs, const ibQuat& rhs ) { return ibQuat::Add(lhs, rhs); }
inline ibQuat operator - ( const ibQuat& lhs, const ibQuat& rhs ) { return ibQuat::Sub(lhs, rhs); }
inline ibQuat operator * ( const ibQuat& lhs, const ibQuat& rhs ) { return ibQuat::Mul(lhs, rhs); }
inline ibQuat operator * ( const ibQuat& lhs, const f32 scalar ) { return ibQuat::Mul(lhs, scalar); }
inline ibVec3 operator * ( const ibQuat& lhs, const ibVec3& rhs ) { return ibQuat::RotatePt(lhs, rhs); }
inline ibVec4 operator * ( const ibQuat& lhs, const ibVec4& rhs ) { return ibQuat::RotatePt(lhs, rhs); }
inline ibQuat operator ~ ( const ibQuat& rhs ) { return rhs.Conjugate(); }

inline bool operator == ( const ibQuat& lhs, const ibQuat& rhs ) {
	return fcmp(lhs.x, rhs.x) && fcmp(lhs.y, rhs.y) && fcmp(lhs.z, rhs.z);
}
inline bool operator != ( const ibQuat& lhs, const ibQuat& rhs ) { return !(lhs == rhs); }

inline ibQuat& operator += ( ibQuat& lhs, const ibQuat& rhs ) { return lhs.Add(rhs); }
inline ibQuat& operator -= ( ibQuat& lhs, const ibQuat& rhs ) { return lhs.Sub(rhs); }
inline ibQuat& operator *= ( ibQuat& lhs, const ibQuat& rhs ) { return lhs.Mul(rhs); }
inline ibQuat& operator *= ( ibQuat& lhs, const f32 rhs ) { return lhs.Mul(rhs); }

#endif // DN_QUAT_H
