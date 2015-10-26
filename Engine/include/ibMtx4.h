#ifndef DN_MATRIX_H
#define DN_MATRIX_H
#pragma once

class ibVec3;
class ibVec4;

class IB_EXPORT ibMtx4
{
public:
	ibMtx4() {} // No default initialization
	ibMtx4( f32 a, f32 b, f32 c, f32 d,
		    f32 e, f32 f, f32 g, f32 h,
		    f32 i, f32 j, f32 k, f32 l,
		    f32 m, f32 n, f32 o, f32 p );
	ibMtx4( const ibMtx4& rhs );
	ibMtx4& operator= ( const ibMtx4& rhs );

	// Members work in place
	ibMtx4& Scale( const f32 f ); // Scalar multiply
	ibMtx4& Translate( const ibVec3& rhs );

	ibMtx4& Add( const ibMtx4& rhs );
	ibMtx4& Sub( const ibMtx4& rhs );
	ibMtx4& Mul( const ibMtx4& rhs );
	ibMtx4& Transpose();
	ibMtx4& Invert();

	ibMtx4& Stabelize();

	// non Member
	static ibMtx4 Scale( const ibMtx4& mtx, const f32 f );
	static ibMtx4 Translate( const ibMtx4& mtx, const ibVec3& vec );

	static ibMtx4 Add( const ibMtx4& lhs, const ibMtx4& rhs );
	static ibMtx4 Sub( const ibMtx4& lhs, const ibMtx4& rhs );
	static ibMtx4 Mul( const ibMtx4& lhs, const ibMtx4& rhs );
	static ibMtx4 Transpose( const ibMtx4& mtx );
	static ibMtx4 Invert( const ibMtx4& mtx );

	static ibMtx4 Stabelize( const ibMtx4& mtx );

	// Data, row major
	union {
		struct {
			f32 _00, _01, _02, _03,
				  _10, _11, _12, _13,
				  _20, _21, _22, _23,
				  _30, _31, _32, _33;
		} mtx;
		f32 a[4][4];
		f32 f[16];
	} data;

	static const ibMtx4 ZERO;
	static const ibMtx4 FLIP_X;
	static const ibMtx4 FLIP_Y;
	static const ibMtx4 FLIP_Z;
	static const ibMtx4 FLIP_W;
	static const ibMtx4 IDENTITY;
};

inline ibMtx4 operator+ ( const ibMtx4& lhs, const ibMtx4& rhs ) { return ibMtx4::Add(lhs, rhs); }
inline ibMtx4 operator- ( const ibMtx4& lhs, const ibMtx4& rhs ) { return ibMtx4::Sub(lhs, rhs); }
inline ibMtx4 operator* ( const f32 scale, const ibMtx4& rhs ) { return ibMtx4::Scale(rhs, scale); }
inline ibMtx4 operator* ( const ibMtx4& lhs, const f32 scale ) { return ibMtx4::Scale(lhs, scale); }
inline ibMtx4 operator* ( const ibMtx4& lhs, const ibMtx4& rhs ) { return ibMtx4::Mul(lhs, rhs); }

inline ibMtx4& operator+= ( ibMtx4& lhs, const ibMtx4& rhs ) { return lhs.Add(rhs); }
inline ibMtx4& operator-= ( ibMtx4& lhs, const ibMtx4& rhs ) { return lhs.Sub(rhs); }
inline ibMtx4& operator*= ( ibMtx4& lhs, const f32 scale ) { return lhs.Scale(scale); }
inline ibMtx4& operator*= ( ibMtx4& lhs, const ibMtx4& rhs ) { return lhs.Mul(rhs); }

#endif // DN_MATRIX_H
