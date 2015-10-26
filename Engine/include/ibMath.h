#ifndef IB_MATH_H
#define IB_MATH_H

#include <cmath>

namespace ibMath
{
	const f32 Pi = 3.14159265f;
}

inline
f32 deg_to_rads(f32 deg)
{
	deg = deg - (floor(deg / 360.f) * 360.f);
	return deg * ibMath::Pi / 180;
}

inline
f32 rads_to_deg(f32 rads)
{
	f32 tau = 2 * ibMath::Pi;
	return rads - (floor(rads / tau) * tau);
}

#define fcmp(a, b) (fabs(a - b) < IB_EPSILON_F)
#define dcmp(a, b) (fabs(a - b) < IB_EPSILON_D)

#endif // IB_MATH_H
