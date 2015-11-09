#include "Stdafx.h"

#include "Vector3.cs.h"
#include "Vector4.cs.h"
#include "Quaternion.cs.h"

namespace Innabus {
	namespace Math
	{
		Vector3^ Quaternion::RotatePt( Vector3^ pt ) 
		{ 
			return gcnew Vector3(m_quat->RotatePt(*pt->m_vector)); 
		}
		Vector4^ Quaternion::RotatePt( Vector4^ pt )
		{ 
			return gcnew Vector4(m_quat->RotatePt(*pt->m_vector)); 
		}
		Vector3^ Quaternion::RotatePt( Quaternion^ q, Vector3^ pt ) { return q->RotatePt(pt); }
		Vector4^ Quaternion::RotatePt( Quaternion^ q, Vector4^ pt ) { return q->RotatePt(pt); }

		Vector3^ Quaternion::operator* (Quaternion^ lhs, Vector3^ rhs)
		{
			return gcnew Vector3(*lhs->m_quat * *rhs->m_vector);
		}
		Vector4^ Quaternion::operator* (Quaternion^ lhs, Vector4^ rhs)
		{
			return gcnew Vector4(*lhs->m_quat * *rhs->m_vector);
		}
	}
}