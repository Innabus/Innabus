#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

#include "ibMtx3.h"

namespace Innabus {
	namespace Math
	{
		ref class Vector3;
		ref class Quaternion;

		public ref class Matrix3
		{
		public:
			Matrix3() { m_matrix = new ibMtx3(); } // No default initialization
			Matrix3( f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, f32 i) { m_matrix = new ibMtx3(a, b, c, d, e, f, g, h, i); }
			Matrix3( const ibMtx3& mtx ) { m_matrix = new ibMtx3(mtx); }
			Matrix3( Matrix3% rhs ) { m_matrix = new ibMtx3(*rhs.m_matrix); }
			Matrix3( Matrix3^ rhs ) { m_matrix = new ibMtx3(*rhs->m_matrix); }
			Matrix3% operator= ( Matrix3% rhs ) { *m_matrix = *rhs.m_matrix; return *this; }

			~Matrix3() { this->!Matrix3(); }
			!Matrix3() { delete m_matrix; }

			// Members work in place
			Matrix3^ Scale( const f32 f ) { m_matrix->Scale(f); return this; }

			Matrix3^ Add( Matrix3^ rhs ) { m_matrix->Add(*rhs->m_matrix); return this; }
			Matrix3^ Sub( Matrix3^ rhs ) { m_matrix->Sub(*rhs->m_matrix); return this; }
			Matrix3^ Mul( Matrix3^ rhs ) { m_matrix->Mul(*rhs->m_matrix); return this; }
			Matrix3^ Transpose() { m_matrix->Transpose(); return this; }
			Matrix3^ Invert() { m_matrix->Invert(); return this; }

			Matrix3^ Stabelize() { m_matrix->Stabelize(); return this; }

			f32 GetElemByIndex(int i) { return m_matrix->data.f[i]; }

			// non Member
			static Matrix3^ Scale( Matrix3^ mtx, const f32 f ) { return (gcnew Matrix3(mtx))->Scale(f); }

			static Matrix3^ Add( Matrix3^ lhs, Matrix3^ rhs ) { return (gcnew Matrix3(lhs))->Add(rhs); }
			static Matrix3^ Sub( Matrix3^ lhs, Matrix3^ rhs ) { return (gcnew Matrix3(lhs))->Sub(rhs); }
			static Matrix3^ Mul( Matrix3^ lhs, Matrix3^ rhs ) { return (gcnew Matrix3(lhs))->Mul(rhs); }
			static Matrix3^ Transpose( Matrix3^ mtx ) { return (gcnew Matrix3(mtx))->Transpose(); }
			static Matrix3^ Invert( Matrix3^ mtx ) { return (gcnew Matrix3(mtx))->Invert(); }

			static Matrix3^ RotateX( f32 angle ) { return gcnew Matrix3(ibMtx3::RotateX(angle)); }
			static Matrix3^ RotateY( f32 angle ) { return gcnew Matrix3(ibMtx3::RotateY(angle)); }
			static Matrix3^ RotateZ( f32 angle ) { return gcnew Matrix3(ibMtx3::RotateZ(angle)); }

			static Matrix3^ RotateEuler( f32 yaw, f32 pitch, f32 roll ) { return gcnew Matrix3(ibMtx3::RotateEuler(yaw, pitch, roll)); }
			static Matrix3^ RotateAxisAngle( Vector3^ axis, f32 angle );
			static Matrix3^ RotateQuaternion( Quaternion^ quat );

			static void GetAxisAngle( Matrix3^ mtx, [Out] Vector3^% axis, [Out] f32% angle );
						
			//static void GetEulerAngles( Matrix3^ mtx, [Out] f32% yaw, [Out] f32% pitch, [Out] f32% roll );

			static Matrix3^ Stabelize( Matrix3^ mtx ) { return (gcnew Matrix3(mtx))->Stabelize(); }

			virtual String^ ToString() override
			{
				return String::Format("[[{0}, {1}, {2}], [{3}, {4}, {5}], [{6} , {7}, {8}]]", 
					m_matrix->data.f[0], m_matrix->data.f[1], m_matrix->data.f[2],
					m_matrix->data.f[3], m_matrix->data.f[4], m_matrix->data.f[5],
					m_matrix->data.f[6], m_matrix->data.f[7], m_matrix->data.f[8]
				);
			}

			ibMtx3* m_matrix;

			static Matrix3^ operator+ ( Matrix3^ lhs, Matrix3^ rhs ) { return Matrix3::Add(lhs, rhs); }
			static Matrix3^ operator- ( Matrix3^ lhs, Matrix3^ rhs ) { return Matrix3::Sub(lhs, rhs); }
			static Matrix3^ operator* ( const f32 scale, Matrix3^ rhs ) { return Matrix3::Scale(rhs, scale); }
			static Matrix3^ operator* ( Matrix3^ lhs, const f32 scale ) { return Matrix3::Scale(lhs, scale); }
			static Matrix3^ operator* ( Matrix3^ lhs, Matrix3^ rhs ) { return Matrix3::Mul(lhs, rhs); }

			static Matrix3^ operator+= ( Matrix3^ lhs, Matrix3^ rhs ) { return lhs->Add(rhs); }
			static Matrix3^ operator-= ( Matrix3^ lhs, Matrix3^ rhs ) { return lhs->Sub(rhs); }
			static Matrix3^ operator*= ( Matrix3^ lhs, const f32 scale ) { return lhs->Scale(scale); }
			static Matrix3^ operator*= ( Matrix3^ lhs, Matrix3^ rhs ) { return lhs->Mul(rhs); }
		};
	}
}
