#ifndef IB_MTX3_H
#define IB_MTX3_H
#pragma once

class ibVec3;
class ibQuat;

class IB_CORE_EXPORT ibMtx3
{
public:
	ibMtx3() {} // No default initialization
	ibMtx3( f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, f32 i);
	ibMtx3( const ibMtx3& rhs );
	ibMtx3& operator= ( const ibMtx3& rhs );

	// Members work in place
	ibMtx3& Scale( const f32 f ); // Scalar multiply

	ibMtx3& Add( const ibMtx3& rhs );
	ibMtx3& Sub( const ibMtx3& rhs );
	ibMtx3& Mul( const ibMtx3& rhs );
	ibMtx3& Transpose();
	ibMtx3& Invert();

	ibMtx3& Stabelize();

	// non Member
	static ibMtx3 Scale( const ibMtx3& mtx, const f32 f );

	static ibMtx3 Add( const ibMtx3& lhs, const ibMtx3& rhs );
	static ibMtx3 Sub( const ibMtx3& lhs, const ibMtx3& rhs );
	static ibMtx3 Mul( const ibMtx3& lhs, const ibMtx3& rhs );
	static ibMtx3 Transpose( const ibMtx3& mtx );
	static ibMtx3 Invert( const ibMtx3& mtx );

	static ibMtx3 RotateX( f32 angle );
	static ibMtx3 RotateY( f32 angle );
	static ibMtx3 RotateZ( f32 angle );

	static ibMtx3 RotateEuler( f32 yaw, f32 pitch, f32 roll );
	static ibMtx3 RotateAxisAngle( ibVec3 axis, const f32 angle );
	static ibMtx3 RotateQuaternion( const ibQuat& quaternion );

	static void GetAxisAngle( const ibMtx3& mtx, ibVec3* pAxis, f32* pAngle );
	//static void GetEulerAngles( const ibMtx3& mtx, f32* yaw, f32* pitch, f32* roll );

	static ibMtx3 Stabelize( const ibMtx3& mtx );

	union
	{
		struct {
			f32 _00, _01, _02,
				_10, _11, _12,
				_20, _21, _22;
		} mtx;
		f32 a[3][3];
		f32 f[9];
	} data;

	static const ibMtx3 ZERO;
	static const ibMtx3 ONE;
	static const ibMtx3 FLIP_X;
	static const ibMtx3 FLIP_Y;
	static const ibMtx3 FLIP_Z;
	static const ibMtx3 IDENTITY;
};

inline ibMtx3 operator+ ( const ibMtx3& lhs, const ibMtx3& rhs ) { return ibMtx3::Add(lhs, rhs); }
inline ibMtx3 operator- ( const ibMtx3& lhs, const ibMtx3& rhs ) { return ibMtx3::Sub(lhs, rhs); }
inline ibMtx3 operator* ( const f32 scale, const ibMtx3& rhs ) { return ibMtx3::Scale(rhs, scale); }
inline ibMtx3 operator* ( const ibMtx3& lhs, const f32 scale ) { return ibMtx3::Scale(lhs, scale); }
inline ibMtx3 operator* ( const ibMtx3& lhs, const ibMtx3& rhs ) { return ibMtx3::Mul(lhs, rhs); }

inline ibMtx3& operator+= ( ibMtx3& lhs, const ibMtx3& rhs ) { return lhs.Add(rhs); }
inline ibMtx3& operator-= ( ibMtx3& lhs, const ibMtx3& rhs ) { return lhs.Sub(rhs); }
inline ibMtx3& operator*= ( ibMtx3& lhs, const f32 scale ) { return lhs.Scale(scale); }
inline ibMtx3& operator*= ( ibMtx3& lhs, const ibMtx3& rhs ) { return lhs.Mul(rhs); }

bool IB_CORE_EXPORT operator== ( const ibMtx3& lhs, const ibMtx3& rhs );
inline bool operator!= ( const ibMtx3& lhs, const ibMtx3& rhs ) { return !(lhs == rhs); }

#endif // IB_MTX3_H
