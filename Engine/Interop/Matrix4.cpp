#include "Stdafx.h"
#include "Matrix4.cs.h"

#include "Vector3.cs.h"
#include "Vector4.cs.h"
#include "Quaternion.cs.h"

namespace Innabus {
	namespace Math
	{
		Matrix4^ Matrix4::RotateAxisAngle( Vector3^ axis, f32 angle )
		{
            return gcnew Matrix4(ibMtx4::RotateAxisAngle(*axis->m_vector, angle));
		}

		Matrix4^ Matrix4::RotateAxisAngle( Vector4^ axis, f32 angle )
		{
            return gcnew Matrix4(ibMtx4::RotateAxisAngle(*axis->m_vector, angle));
		}

		Matrix4^ Matrix4::RotateQuaternion( Quaternion^ quat )
		{
			return gcnew Matrix4(ibMtx4::RotateQuaternion(*quat->m_quat));
		}

		void Matrix4::GetAxisAngle( Matrix4^ mtx, Vector3^% axis, f32% angle )
		{
			ibVec3 v;
			f32 f;

			ibMtx4::GetAxisAngle( *mtx->m_matrix, &v, &f );
			axis = gcnew Vector3(v);
			angle = f;
		}

		void Matrix4::GetAxisAngle( Matrix4^ mtx, Vector4^% axis, f32% angle )
		{
			ibVec4 v;
			f32 f;

			ibMtx4::GetAxisAngle( *mtx->m_matrix, &v, &f );
			axis = gcnew Vector4(v);
			angle = f;
		}

		//void Matrix4::GetEulerAngles( Matrix4^ mtx, [Out] f32% yaw, [Out] f32% pitch, [Out] f32% roll )
		//{
		//	f32 y, p, r;
		//	ibMtx3::GetEulerAngles( *mtx->m_matrix, &y, &p, &r );
		//	yaw = y;
		//	pitch = p;
		//	roll = r;
		//}
	}
}