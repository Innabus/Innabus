#ifndef IB_MTX2_H
#define IB_MTX2_H
#pragma once

class IB_EXPORT ibMtx2
{
public:
	ibMtx2() {} // No default initialization
	ibMtx2( f32 a, f32 b, f32 c, f32 d);
	ibMtx2( const ibMtx2& rhs );
	ibMtx2& operator= ( const ibMtx2& rhs );

	// Members work in place
	ibMtx2& Scale( const float f ); // Scalar multiply

	ibMtx2& Add( const ibMtx2& rhs );
	ibMtx2& Sub( const ibMtx2& rhs );
	ibMtx2& Mul( const ibMtx2& rhs );
	ibMtx2& Transpose();
	ibMtx2& Invert();

	ibMtx2& Stabelize();

	// non Member
	static ibMtx2 Scale( const ibMtx2& mtx, const f32 f );

	static ibMtx2 Add( const ibMtx2& lhs, const ibMtx2& rhs );
	static ibMtx2 Sub( const ibMtx2& lhs, const ibMtx2& rhs );
	static ibMtx2 Mul( const ibMtx2& lhs, const ibMtx2& rhs );
	static ibMtx2 Transpose( const ibMtx2& mtx );
	static ibMtx2 Invert( const ibMtx2& mtx );

	static ibMtx2 Rotation( f32 angle );

	static ibMtx2 Stabelize( const ibMtx2& mtx );

	union
	{
		struct {
			f32 _00, _01,
				_10, _11;
		} mtx;
		f32 a[2][2];
		f32 f[4];
	} data;

	static const ibMtx2 ZERO;
	static const ibMtx2 ONE;
	static const ibMtx2 FLIP_X;
	static const ibMtx2 FLIP_Y;
	static const ibMtx2 IDENTITY;
};

#endif // IB_MTX2_H
