#include "ibMtx3.h"

#include <cmath>

#include "ibVec3.h"
#include "ibMtx4.h"
#include "ibQuat.h"

#include "ibMemory.h"

const ibMtx3 ibMtx3::ZERO     = ibMtx3( 0,  0,  0,  0,  0,  0,  0,  0,  0 );
const ibMtx3 ibMtx3::ONE      = ibMtx3( 1,  1,  1,  1,  1,  1,  1,  1,  1 );
const ibMtx3 ibMtx3::FLIP_X   = ibMtx3(-1,  0,  0,  0,  1,  0,  0,  0,  1 );
const ibMtx3 ibMtx3::FLIP_Y   = ibMtx3( 1,  0,  0,  0, -1,  0,  0,  0,  1 );
const ibMtx3 ibMtx3::FLIP_Z   = ibMtx3( 1,  0,  0,  0,  1,  0,  0,  0, -1 );
const ibMtx3 ibMtx3::IDENTITY = ibMtx3( 1,  0,  0,  0,  1,  0,  0,  0,  1 );

ibMtx3::ibMtx3( f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, f32 i)
{
	data.mtx._00 = a; data.mtx._01 = b; data.mtx._02 = c;
	data.mtx._10 = d; data.mtx._11 = e; data.mtx._12 = f;
	data.mtx._20 = g; data.mtx._21 = h; data.mtx._22 = i;
}

ibMtx3::ibMtx3( const ibMtx3& rhs )
{
	for (u32 n = 0; n < 9; ++n)
	{
		data.f[n] = rhs.data.f[n];
	}
}

ibMtx3& ibMtx3::operator= ( const ibMtx3& rhs )
{
	for (u32 n = 0; n < 9; ++n)
	{
		data.f[n] = rhs.data.f[n];
	}
	return *this;
}

// Members work in place
ibMtx3& ibMtx3::Scale( const f32 f )
{
	for (u32 n = 0; n < 9; ++n)
	{
		data.f[n] *= f;
	}
	return *this;
}

ibMtx3& ibMtx3::Add( const ibMtx3& rhs )
{
	for (u32 n = 0; n < 9; ++n)
	{
		data.f[n] += rhs.data.f[n];
	}
	return *this;
}

ibMtx3& ibMtx3::Sub( const ibMtx3& rhs )
{
	for (u32 n = 0; n < 9; ++n)
	{
		data.f[n] -= rhs.data.f[n];
	}
	return *this;
}

ibMtx3& ibMtx3::Mul( const ibMtx3& rhs )
{
	f32 f[3];
	for (unsigned row = 0; row < 3; ++row)
	{
		ibMemcpy(f, data.a[row], sizeof(f32) * 3);
		for (u32 n = 0; n < 3; ++n)
		{
			data.a[row][n] = f[0] * rhs.data.a[0][n] + f[1] * rhs.data.a[1][n] + f[2] * rhs.data.a[2][n];
		}
	}
	return *this;
}

ibMtx3& ibMtx3::Transpose()
{
	// 01 x 10
	f32 tmp = data.mtx._01;
	data.mtx._01 = data.mtx._10;
	data.mtx._10 = tmp;

	// 02 x 20
	tmp = data.mtx._02;
	data.mtx._02 = data.mtx._20;
	data.mtx._20 = tmp;

	// 12 x 21
	tmp = data.mtx._12;
	data.mtx._12 = data.mtx._21;
	data.mtx._21 = tmp;

	return *this;
}

ibMtx3& ibMtx3::Invert()
{
	return (*this = ibMtx3::Invert(*this));
}

ibMtx3& ibMtx3::Stabelize()
{
	for (u32 n = 0; n < 9; ++n)
	{
		if (fabs(data.f[n]) < IB_EPSILON_F)
			data.f[n] = 0;
	}
	return *this;
}

// non Member
ibMtx3 ibMtx3::Scale( const ibMtx3& mtx, const f32 f )
{
	return ibMtx3(mtx).Scale(f);
}

ibMtx3 ibMtx3::Add( const ibMtx3& lhs, const ibMtx3& rhs )
{
	return ibMtx3(lhs).Add(rhs);
}

ibMtx3 ibMtx3::Sub( const ibMtx3& lhs, const ibMtx3& rhs )
{
	return ibMtx3(lhs).Sub(rhs);
}

ibMtx3 ibMtx3::Mul( const ibMtx3& lhs, const ibMtx3& rhs )
{
	return ibMtx3(lhs).Mul(rhs);
}

ibMtx3 ibMtx3::Transpose( const ibMtx3& mtx )
{
	return ibMtx3(mtx).Transpose();
}

ibMtx3 ibMtx3::Invert( const ibMtx3& mtx )
{
	auto& m = mtx.data.mtx;
	f32 det = m._00 * ((m._11 * m._22) - (m._12 * m._21))
		    - m._01 * ((m._10 * m._22) - (m._12 * m._20))
			+ m._02 * ((m._10 * m._21) - (m._11 * m._20));
	f32 det_recip = 1.f / det;
	if (det)
	{
		ibMtx3 result;
		f32* f = result.data.f;
		f[0] =  (m._11 * m._22 - m._12 * m._21);
		f[1] = -(m._01 * m._22 - m._02 * m._21);
		f[2] =  (m._01 * m._12 - m._02 * m._11);
		f[3] = -(m._10 * m._22 - m._12 * m._20);
		f[4] =  (m._00 * m._22 - m._02 * m._20);
		f[5] = -(m._00 * m._12 - m._10 * m._02);
		f[6] =  (m._10 * m._21 - m._20 * m._11);
		f[7] = -(m._00 * m._21 - m._20 * m._01);
		f[8] =  (m._00 * m._11 - m._10 * m._01);
		for (u32 n = 0; n < 9; ++n)
			f[n] *= det_recip;
		return result;
	}
	return ibMtx3::ZERO;
}

ibMtx3 ibMtx3::RotateX( f32 angle )
{
	return ibMtx3(1,      0,         0,
		          0,  cos(angle), sin(angle),
				  0, -sin(angle), cos(angle));
}

ibMtx3 ibMtx3::RotateY( f32 angle )
{
	return ibMtx3(cos(angle), 0, -sin(angle),
		              0,      1,      0,
			      sin(angle), 0,  cos(angle));
}

ibMtx3 ibMtx3::RotateZ( f32 angle )
{
	return ibMtx3( cos(angle), sin(angle), 0,
		          -sin(angle), cos(angle), 0,
				       0,          0,      1);
}

ibMtx3 ibMtx3::RotateAxisAngle( ibVec3 v, const f32 angle )
{
	f32 sina = sin(angle);
	f32 cosa = cos(angle);
	f32 cosa_1 = 1 - cosa;
	v.Normalize();
	f32 x2 = v.x * v.x * cosa_1, y2 = v.y * v.y * cosa_1, z2 = v.z * v.z * cosa_1;
	f32 xy = v.x * v.y * cosa_1, xz = v.x * v.z * cosa_1, yz = v.y * v.z * cosa_1;
	f32 xsin = v.x * sina, ysin = v.y * sina, zsin = v.z * sina;
	return ibMtx3(cosa + x2, xy + zsin, xz - ysin,
		          xy - zsin, cosa + y2, yz + xsin,
				  xz + ysin, yz - xsin, cosa + z2);
}

void ibMtx3::GetAxisAngle( const ibMtx3& mtx, ibVec3* pAxis, f32* pAngle )
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

ibMtx3 ibMtx3::RotateEuler( f32 yaw, f32 pitch, f32 roll )
{
	f32 cy = cos(yaw), sy = sin(yaw);
	f32 cp = cos(pitch), sp = sin(pitch);
	f32 cr = cos(roll), sr = sin(roll);

	return ibMtx3( cr*cy + sr*sp*sy, sr * cp, sr*sp*cy - cr*sy,
		           cr*sp*sy - sr*cy, cr * cp, sr*sy + cr*sp*cy,
				         cp * sy   ,   -sp  ,    cp * cy      );
}

ibMtx3 ibMtx3::RotateQuaternion( const ibQuat& quat )
{
	ibMtx4 m = quat.GetMatrix();
	return ibMtx3(m.data.mtx._00, m.data.mtx._01, m.data.mtx._02,
		          m.data.mtx._10, m.data.mtx._11, m.data.mtx._12,
				  m.data.mtx._20, m.data.mtx._21, m.data.mtx._22);
}

/*
void ibMtx3::GetEulerAngles( const ibMtx3& mtx, f32* yaw, f32* pitch, f32* roll )
{
	mtx;
	if (yaw) *yaw = 0;
	if (pitch) *pitch = 0;
	if (roll) * roll = 0;
}
*/

ibMtx3 ibMtx3::Stabelize( const ibMtx3& mtx )
{
	return ibMtx3(mtx).Stabelize();
}

bool IB_CORE_EXPORT operator== ( const ibMtx3& lhs, const ibMtx3& rhs )
{
	for (u32 n = 0; n < 9; ++n)
	{
		if (!fcmp(lhs.data.f[n], rhs.data.f[n]))
			return false;
	}
	return true;
}
