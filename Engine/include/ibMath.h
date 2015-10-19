#ifndef IB_MATH_H
#define IB_MATH_H

#include "ibEngineDefines.h"

namespace ibMath
{
	const float Pi = 3.14159265f;
}

#define fcmp(a, b) (fabs(a - b) < IB_EPSILON_F)
#define dcmp(a, b) (fabs(a - b) < IB_EPSILON_D)

#endif // IB_MATH_H
