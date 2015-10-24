#include "ibMtx4.h"
#include "ibVec3.h"

#include <memory.h>

#include <emmintrin.h>

namespace
{
	void fswap( float& l, float& r )
	{
		float tmp = l;
		l = r;
		r = tmp;
	}
}

const ibMtx ibMtx::ZERO     = ibMtx( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
const ibMtx ibMtx::IDENTITY = ibMtx( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );

ibMtx::ibMtx( 
	float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float m, float n, float o, float p )
{
	float* pf = data.a[0];
	*pf++ = a; *pf++ = b; *pf++ = c; *pf++ = d;
	*pf++ = e; *pf++ = f; *pf++ = g; *pf++ = h;
	*pf++ = i; *pf++ = j; *pf++ = k; *pf++ = l;
	*pf++ = m; *pf++ = n; *pf++ = o; *pf++ = p;
}

ibMtx::ibMtx( const ibMtx& rhs )
{
	float* to = data.a[0];
	const float* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ = *from++;
}

ibMtx& ibMtx::operator= ( const ibMtx& rhs )
{
	float* to = data.a[0];
	const float* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ = *from++;
	return *this;
}

// Members work in place
ibMtx& ibMtx::Scale( const float f ) // Scalar multiply
{
	float* to = data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ *= f;
	return *this;
}

ibMtx& ibMtx::ScaleAxes( const ibVec3& v ) // Scale on all three axis
{
	data.mtx._00 *= v.x;
	data.mtx._11 *= v.y;
	data.mtx._22 *= v.z;
	return *this;
}

ibMtx& ibMtx::Translate( const ibVec3& rhs )
{
	data.mtx._03 += rhs.x;
	data.mtx._13 += rhs.y;
	data.mtx._23 += rhs.z;
	return *this;
}

ibMtx& ibMtx::Add( const ibMtx& rhs )
{
	float* to = data.a[0];
	const float* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ += *from++;
	return *this;
}

ibMtx& ibMtx::Sub( const ibMtx& rhs )
{
	float* to = data.a[0];
	const float* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ -= *from++;
	return *this;
}

ibMtx& ibMtx::Mul( const ibMtx& rhs )
{
	float f[4];
	for ( unsigned row = 0; row < 4; ++row )
	{
		memcpy( f, data.a[row], sizeof(float) * 4 );
		for ( unsigned n = 0; n < 4; ++n )
			data.a[row][n] = f[0] * rhs.data.a[0][n] + f[1] * rhs.data.a[1][n] + f[2] * rhs.data.a[2][n] + f[3] * rhs.data.a[3][n];
	}
	return *this;
}

ibMtx& ibMtx::Transpose()
{
	fswap( data.mtx._01, data.mtx._10 );
	fswap( data.mtx._02, data.mtx._20 );
	fswap( data.mtx._03, data.mtx._30 );
	fswap( data.mtx._12, data.mtx._21 );
	fswap( data.mtx._13, data.mtx._31 );
	fswap( data.mtx._23, data.mtx._32 );
	return *this;
}

#pragma warning ( push )
#pragma warning ( disable: 4700 )
ibMtx& ibMtx::Invert()
{
	float* src = &data.a[0][0];
	__m128 minor0, minor1, minor2, minor3;
	__m128 row0, row1, row2, row3;
	__m128 det, tmp1;
#if !defined NDEBUG || defined STATIC
	// Suppress RTC error for uninit vars
	float init = 0.f;
	row3 = row1 = tmp1 = _mm_load_ps1( &init );
#endif // NDEBUG
	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row2, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0 = _mm_mul_ps(row1, tmp1);
	minor1 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row1, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	minor3 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2 = _mm_shuffle_ps(row2, row2, 0x4E);
	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	minor2 = _mm_mul_ps(row0, tmp1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row0, row1);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row0, row3);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
	// -----------------------------------------------
	tmp1 = _mm_mul_ps(row0, row2);
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
	// -----------------------------------------------
	det = _mm_mul_ps(row0, minor0);
	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	tmp1 = _mm_rcp_ss(det);
	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det = _mm_shuffle_ps(det, det, 0x00);
	minor0 = _mm_mul_ps(det, minor0);
	_mm_storel_pi((__m64*)(src), minor0);
	_mm_storeh_pi((__m64*)(src+2), minor0);
	minor1 = _mm_mul_ps(det, minor1);
	_mm_storel_pi((__m64*)(src+4), minor1);
	_mm_storeh_pi((__m64*)(src+6), minor1);
	minor2 = _mm_mul_ps(det, minor2);
	_mm_storel_pi((__m64*)(src+ 8), minor2);
	_mm_storeh_pi((__m64*)(src+10), minor2);
	minor3 = _mm_mul_ps(det, minor3);
	_mm_storel_pi((__m64*)(src+12), minor3);
	_mm_storeh_pi((__m64*)(src+14), minor3);

	return *this;
}
#pragma warning ( pop )

// non Member
ibMtx ibMtx::Scale( const ibMtx& mtx, const float f )
{
	ibMtx ret( mtx );
	return ret.Scale( f );
}

ibMtx ibMtx::ScaleAxes( const ibMtx& mtx, const ibVec3& vec )
{
	ibMtx ret( mtx );
	return ret.ScaleAxes( vec );
}

ibMtx ibMtx::Translate( const ibMtx& mtx, const ibVec3& vec )
{
	ibMtx ret( mtx );
	return ret.Translate( vec );
}

ibMtx ibMtx::Add( const ibMtx& lhs, const ibMtx& rhs )
{
	ibMtx ret( lhs );
	return ret.Add( rhs );
}

ibMtx ibMtx::Sub( const ibMtx& lhs, const ibMtx& rhs )
{
	ibMtx ret( lhs );
	return ret.Sub( rhs );
}

ibMtx ibMtx::Mul( const ibMtx& lhs, const ibMtx& rhs )
{
	ibMtx ret( lhs );
	return ret.Mul( rhs );
}

ibMtx ibMtx::Transpose( const ibMtx& mtx )
{
	ibMtx ret( mtx );
	return ret.Transpose();
}

ibMtx ibMtx::Invert( const ibMtx& mtx )
{
	ibMtx ret(mtx);
	return ret.Invert();
}
