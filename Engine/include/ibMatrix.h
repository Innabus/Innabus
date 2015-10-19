#ifndef DN_MATRIX_H
#define DN_MATRIX_H
#pragma once

class ibVec3;

class ibMtx
{
public:
	ibMtx() {} // No default initialization
	ibMtx( float a, float b, float c, float d,
		   float e, float f, float g, float h,
		   float i, float j, float k, float l,
		   float m, float n, float o, float p );
	ibMtx( const ibMtx& rhs );
	ibMtx& operator= ( const ibMtx& rhs );

	// Members work in place
	ibMtx& Scale( const float f ); // Scalar multiply
	ibMtx& ScaleAxes( const ibVec3& v ); // Scale on all three axis
	ibMtx& Translate( const ibVec3& rhs );

	ibMtx& Add( const ibMtx& rhs );
	ibMtx& Sub( const ibMtx& rhs );
	ibMtx& Mul( const ibMtx& rhs );
	ibMtx& Transpose();
	ibMtx& Invert();

	// non Member
	static ibMtx Scale( const ibMtx& mtx, const float f );
	static ibMtx ScaleAxes( const ibMtx& mtx, const ibVec3& vec );
	static ibMtx Translate( const ibMtx& mtx, const ibVec3& vec );

	static ibMtx Add( const ibMtx& lhs, const ibMtx& rhs );
	static ibMtx Sub( const ibMtx& lhs, const ibMtx& rhs );
	static ibMtx Mul( const ibMtx& lhs, const ibMtx& rhs );
	static ibMtx Transpose( const ibMtx& mtx );
	static ibMtx Invert( const ibMtx& mtx );

	// Data, row major
	union {
		struct {
			float _00, _01, _02, _03,
				  _10, _11, _12, _13,
				  _20, _21, _22, _23,
				  _30, _31, _32, _33;
		} mtx;
		float a[4][4];
	} data;

	static const ibMtx ZERO;
	static const ibMtx IDENTITY;
};

#endif // DN_MATRIX_H
