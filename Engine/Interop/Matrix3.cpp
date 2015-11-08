#include "Stdafx.h"
#include "Matrix3.cs.h"

#include "Vector3.cs.h"
#include "Quaternion.cs.h"

namespace Innabus {
	namespace Math
	{
		Matrix3^ Matrix3::RotateAxisAngle( Vector3^ axis, f32 angle )
		{
            return gcnew Matrix3(ibMtx3::RotateAxisAngle(*axis->m_vector, angle));
		}

		Matrix3^ Matrix3::RotateQuaternion( Quaternion^ quat )
		{
			return gcnew Matrix3(ibMtx3::RotateQuaternion(*quat->m_quat));
		}

		void Matrix3::GetAxisAngle( Matrix3^ mtx, Vector3^% axis, f32% angle )
		{
			ibVec3 v;
			f32 f;

			ibMtx3::GetAxisAngle( *mtx->m_matrix, &v, &f );
			axis = gcnew Vector3(v);
			angle = f;
		}

		//void Matrix3::GetEulerAngles( Matrix3^ mtx, [Out] f32% yaw, [Out] f32% pitch, [Out] f32% roll )
		//{
		//	f32 y, p, r;
		//	ibMtx3::GetEulerAngles( *mtx->m_matrix, &y, &p, &r );
		//	yaw = y;
		//	pitch = p;
		//	roll = r;
		//}
	}
}