#include "ibMtx4.h"
#include "ibVec3.h"
#include "ibVec4.h"
#include "ibQuat.h"

//#include <memory.h>

#include <emmintrin.h>

#include "ibMemory.h"

namespace
{
	void fswap( f32& l, f32& r )
	{
		f32 tmp = l;
		l = r;
		r = tmp;
	}
}

const ibMtx4 ibMtx4::ZERO     = ibMtx4( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
const ibMtx4 ibMtx4::IDENTITY = ibMtx4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );

ibMtx4::ibMtx4( 
	f32 a, f32 b, f32 c, f32 d,
	f32 e, f32 f, f32 g, f32 h,
	f32 i, f32 j, f32 k, f32 l,
	f32 m, f32 n, f32 o, f32 p )
{
	f32* pf = data.a[0];
	*pf++ = a; *pf++ = b; *pf++ = c; *pf++ = d;
	*pf++ = e; *pf++ = f; *pf++ = g; *pf++ = h;
	*pf++ = i; *pf++ = j; *pf++ = k; *pf++ = l;
	*pf++ = m; *pf++ = n; *pf++ = o; *pf++ = p;
}

ibMtx4::ibMtx4( const ibMtx4& rhs )
{
	f32* to = data.a[0];
	const f32* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ = *from++;
}

ibMtx4& ibMtx4::operator= ( const ibMtx4& rhs )
{
	f32* to = data.a[0];
	const f32* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ = *from++;
	return *this;
}

// Members work in place
ibMtx4& ibMtx4::Scale( const f32 f ) // Scalar multiply
{
	f32* to = data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ *= f;
	return *this;
}

ibMtx4& ibMtx4::Translate( const ibVec3& rhs )
{
	data.mtx._03 += rhs.x;
	data.mtx._13 += rhs.y;
	data.mtx._23 += rhs.z;
	return *this;
}

ibMtx4& ibMtx4::Add( const ibMtx4& rhs )
{
	f32* to = data.a[0];
	const f32* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ += *from++;
	return *this;
}

ibMtx4& ibMtx4::Sub( const ibMtx4& rhs )
{
	f32* to = data.a[0];
	const f32* from = rhs.data.a[0];
	for ( unsigned n = 0; n < 16; ++n )
		*to++ -= *from++;
	return *this;
}

ibMtx4& ibMtx4::Mul( const ibMtx4& rhs )
{
	f32 f[4];
	for ( unsigned row = 0; row < 4; ++row )
	{
		ibMemcpy( f, data.a[row], sizeof(f32) * 4 );
		for ( unsigned n = 0; n < 4; ++n )
			data.a[row][n] = f[0] * rhs.data.a[0][n] + f[1] * rhs.data.a[1][n] + f[2] * rhs.data.a[2][n] + f[3] * rhs.data.a[3][n];
	}
	return *this;
}

ibMtx4& ibMtx4::Transpose()
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
ibMtx4& ibMtx4::Invert()
{
	f32* src = &data.a[0][0];
	__m128 minor0, minor1, minor2, minor3;
	__m128 row0, row1, row2, row3;
	__m128 det, tmp1;
#if !defined NDEBUG || defined STATIC
	// Suppress RTC error for uninit vars
	f32 init = 0.f;
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

ibMtx4& ibMtx4::Stabelize()
{
	for (u32 n = 0; n < 16; ++n)
	{
		if (fabs(data.f[n]) < IB_EPSILON_F)
			data.f[n] = 0;
	}
	return *this;
}

// non Member
ibMtx4 ibMtx4::Scale( const ibMtx4& mtx, const f32 f )
{
	ibMtx4 ret( mtx );
	return ret.Scale( f );
}

ibMtx4 ibMtx4::Translate( const ibMtx4& mtx, const ibVec3& vec )
{
	ibMtx4 ret( mtx );
	return ret.Translate( vec );
}

ibMtx4 ibMtx4::Add( const ibMtx4& lhs, const ibMtx4& rhs )
{
	ibMtx4 ret( lhs );
	return ret.Add( rhs );
}

ibMtx4 ibMtx4::Sub( const ibMtx4& lhs, const ibMtx4& rhs )
{
	ibMtx4 ret( lhs );
	return ret.Sub( rhs );
}

ibMtx4 ibMtx4::Mul( const ibMtx4& lhs, const ibMtx4& rhs )
{
	ibMtx4 ret( lhs );
	return ret.Mul( rhs );
}

ibMtx4 ibMtx4::Transpose( const ibMtx4& mtx )
{
	ibMtx4 ret( mtx );
	return ret.Transpose();
}

ibMtx4 ibMtx4::Invert( const ibMtx4& mtx )
{
	ibMtx4 ret(mtx);
	return ret.Invert();
}


ibMtx4 ibMtx4::RotateX( f32 angle )
{
	return ibMtx4(1,      0,         0,       0,
		          0,  cos(angle), sin(angle), 0,
				  0, -sin(angle), cos(angle), 0,
				  0,     0,          0,       1);
}

ibMtx4 ibMtx4::RotateY( f32 angle )
{
	return ibMtx4(cos(angle), 0, -sin(angle), 0,
		              0,      1,      0,      0,
			      sin(angle), 0,  cos(angle), 0,
				      0,      0,      0,      1);
}

ibMtx4 ibMtx4::RotateZ( f32 angle )
{
	return ibMtx4( cos(angle), sin(angle), 0, 0,
		          -sin(angle), cos(angle), 0, 0,
				       0,          0,      1, 0,
					   0,          0,      0, 1);
}

ibMtx4 ibMtx4::RotateAxisAngle( ibVec3 v, const f32 angle )
{
	f32 sina = sin(angle);
	f32 cosa = cos(angle);
	f32 cosa_1 = 1 - cosa;
	v.Normalize();
	f32 x2 = v.x * v.x * cosa_1, y2 = v.y * v.y * cosa_1, z2 = v.z * v.z * cosa_1;
	f32 xy = v.x * v.y * cosa_1, xz = v.x * v.z * cosa_1, yz = v.y * v.z * cosa_1;
	f32 xsin = v.x * sina, ysin = v.y * sina, zsin = v.z * sina;
	return ibMtx4(cosa + x2, xy + zsin, xz - ysin, 0,
		          xy - zsin, cosa + y2, yz + xsin, 0,
				  xz + ysin, yz - xsin, cosa + z2, 0,
				      0,         0,         0,     1);
}

ibMtx4 ibMtx4::RotateAxisAngle( ibVec4 axis, const f32 angle )
{
	return RotateAxisAngle(ibVec3(axis.x, axis.y, axis.z), angle);
}

void ibMtx4::GetAxisAngle( const ibMtx4& mtx, ibVec3* pAxis, f32* pAngle )
{
	if (pAngle)
		*pAngle = acos((mtx.data.mtx._00 + mtx.data.mtx._11 + mtx.data.mtx._22 - 1) / 2);
	if (pAxis)
	{
		pAxis->x = mtx.data.mtx._12 - mtx.data.mtx._21;
		pAxis->y = mtx.data.mtx._20 - mtx.data.mtx._02;
		pAxis->z = mtx.data.mtx._01 - mtx.data.mtx._10;
		pAxis->Normalize();
	}
}

void ibMtx4::GetAxisAngle( const ibMtx4& mtx, ibVec4* pAxis, f32* pAngle )
{
	ibVec3 v;
	f32 a;
	GetAxisAngle( mtx, &v, &a );
	if (pAxis)
		*pAxis = ibVec4(v.x, v.y, v.z, 0);
	if (pAngle)
		*pAngle = a;
}

ibMtx4 ibMtx4::RotateEuler( f32 yaw, f32 pitch, f32 roll )
{
	f32 cy = cos(yaw), sy = sin(yaw);
	f32 cp = cos(pitch), sp = sin(pitch);
	f32 cr = cos(roll), sr = sin(roll);

	return ibMtx4( cr*cy + sr*sp*sy, sr * cp, sr*sp*cy - cr*sy, 0,
		           cr*sp*sy - sr*cy, cr * cp, sr*sy + cr*sp*cy, 0,
				         cp * sy   ,   -sp  ,    cp * cy      , 0,
						    0      ,    0   ,       0         , 1);
}

ibMtx4 ibMtx4::RotateQuaternion( const ibQuat& quat )
{
	return quat.GetMatrix();
}

ibMtx4 ibMtx4::Stabelize( const ibMtx4& mtx )
{
	return ibMtx4(mtx).Stabelize();
}

bool IB_CORE_EXPORT operator== ( const ibMtx4& lhs, const ibMtx4& rhs )
{
	for (u32 n = 0; n < 16; ++n)
	{
		if (!fcmp(lhs.data.f[n], rhs.data.f[n]))
			return false;
	}
	return true;
}
