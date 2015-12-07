#include "ibMtx2.h"

#include <cmath>

#include "ibMath.h"

const ibMtx2 ibMtx2::ZERO     = ibMtx2(  0,  0,  0,  0 );
const ibMtx2 ibMtx2::ONE      = ibMtx2(  1,  1,  1,  1 );
const ibMtx2 ibMtx2::FLIP_X   = ibMtx2( -1,  0,  0,  1 );
const ibMtx2 ibMtx2::FLIP_Y   = ibMtx2(  1,  0,  0, -1 );
const ibMtx2 ibMtx2::IDENTITY = ibMtx2(  1,  0,  0,  1 );

ibMtx2::ibMtx2( f32 a, f32 b, f32 c, f32 d)
{
	data.mtx._00 = a;
	data.mtx._01 = b;
	data.mtx._10 = c;
	data.mtx._11 = d;
}

ibMtx2::ibMtx2( const ibMtx2& rhs )
{
	*this = rhs;
}

ibMtx2& ibMtx2::operator= ( const ibMtx2& rhs )
{
	data.mtx._00 = rhs.data.mtx._00;
	data.mtx._01 = rhs.data.mtx._01;
	data.mtx._10 = rhs.data.mtx._10;
	data.mtx._11 = rhs.data.mtx._11;
	return *this;
}

// Members work in place
ibMtx2& ibMtx2::Scale( const f32 f )
{
	for (u32 n = 0; n < 4; ++n)
		data.f[n] *= f;
	return *this;
}

ibMtx2& ibMtx2::Add( const ibMtx2& rhs )
{
	for (u32 n = 0; n < 4; ++n)
		data.f[n] += rhs.data.f[n];
	return *this;
}

ibMtx2& ibMtx2::Sub( const ibMtx2& rhs )
{
	for (u32 n = 0; n < 4; ++n)
		data.f[n] -= rhs.data.f[n];
	return *this;
}

ibMtx2& ibMtx2::Mul( const ibMtx2& rhs )
{
	f32 tmp[4] = {};
	tmp[0] = (data.mtx._00 * rhs.data.mtx._00) + (data.mtx._01 * rhs.data.mtx._10);
	tmp[1] = (data.mtx._00 * rhs.data.mtx._01) + (data.mtx._01 * rhs.data.mtx._11);
	tmp[2] = (data.mtx._10 * rhs.data.mtx._00) + (data.mtx._11 * rhs.data.mtx._10);
	tmp[3] = (data.mtx._10 * rhs.data.mtx._01) + (data.mtx._11 * rhs.data.mtx._11);
	for (u32 n = 0; n < 4; ++n)
		data.f[n] = tmp[n];
	return *this;
}

ibMtx2& ibMtx2::Transpose()
{
	f32 tmp = data.mtx._01;
	data.mtx._01 = data.mtx._10;
	data.mtx._10 = tmp;
	return *this;
}

ibMtx2& ibMtx2::Invert()
{
	return (*this = ibMtx2::Invert(*this));
}

ibMtx2& ibMtx2::Stabelize()
{
	for (u32 n = 0; n < 4; ++n)
	{
		if (fabs(data.f[n]) < IB_EPSILON_F)
			data.f[n] = 0.f;
	}
	return *this;
}

// non Member
ibMtx2 ibMtx2::Scale( const ibMtx2& mtx, const f32 f )
{
	return ibMtx2(mtx).Scale(f);
}

ibMtx2 ibMtx2::Add( const ibMtx2& lhs, const ibMtx2& rhs )
{
	return ibMtx2(lhs).Add(rhs);
}

ibMtx2 ibMtx2::Sub( const ibMtx2& lhs, const ibMtx2& rhs )
{
	return ibMtx2(lhs).Sub(rhs);
}

ibMtx2 ibMtx2::Mul( const ibMtx2& lhs, const ibMtx2& rhs )
{
	return ibMtx2(lhs).Mul(rhs);
}

ibMtx2 ibMtx2::Transpose( const ibMtx2& mtx )
{
	return ibMtx2(mtx).Transpose();
}

ibMtx2 ibMtx2::Invert( const ibMtx2& mtx )
{
	f32 det = (mtx.data.mtx._00 * mtx.data.mtx._11) - (mtx.data.mtx._01 * mtx.data.mtx._10);
	if (det)
	{
		ibMtx2 ret = ibMtx2(mtx.data.mtx._11, -mtx.data.mtx._01, -mtx.data.mtx._10, mtx.data.mtx._00);
		for (u32 n = 0; n < 4; ++n)
			ret.data.f[n] /= det;
		return ret;
	}
	return ibMtx2::ZERO;
}

ibMtx2 ibMtx2::Rotation( f32 angle )
{
	return ibMtx2(
		cos(angle), sin(angle),
		-sin(angle), cos(angle)
	);
}

ibMtx2 ibMtx2::Stabelize( const ibMtx2& mtx )
{
	return ibMtx2(mtx).Stabelize();
}

bool IB_CORE_EXPORT operator== ( const ibMtx2& lhs, const ibMtx2& rhs )
{
	for (u32 n = 0; n < 4; ++n)
	{
		if (!fcmp(lhs.data.f[n], rhs.data.f[n]))
			return false;
	}
	return true;
}
