#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

#include "ibMtx4.h"

namespace Innabus {
	namespace Math
	{
		ref class Vector3;
		ref class Vector4;
		ref class Quaternion;

		public ref class Matrix4
		{
		public:
			Matrix4() { m_matrix = new ibMtx4(); } // No default initialization
			Matrix4( f32 a, f32 b, f32 c, f32 d, 
				     f32 e, f32 f, f32 g, f32 h,
					 f32 i, f32 j, f32 k, f32 l,
					 f32 m, f32 n, f32 o, f32 p ) { 
				m_matrix = new ibMtx4(a, b, c, d, 
					                  e, f, g, h,
									  i, j, k, l,
									  m, n, o, p); }
			Matrix4( const ibMtx4& rhs ) { m_matrix = new ibMtx4(rhs); }
			Matrix4( Matrix4% rhs ) { m_matrix = new ibMtx4(*rhs.m_matrix); }
			Matrix4( Matrix4^ rhs ) { m_matrix = new ibMtx4(*rhs->m_matrix); }
			Matrix4% operator= ( Matrix4% rhs ) { *m_matrix = *rhs.m_matrix; return *this; }

			~Matrix4() { this->!Matrix4(); }
			!Matrix4() { delete m_matrix; }

			virtual String^ ToString() override
			{
				return String::Format("[[{0}, {1}, {2}, {3}], [{4}, {5}, {6},{7}],"
					                  " [{8}, {9}, {10}, {11}], [{12}, {13}, {14}, {15}]]",
					m_matrix->data.f[0], m_matrix->data.f[1], m_matrix->data.f[2], m_matrix->data.f[3],
					m_matrix->data.f[4], m_matrix->data.f[5], m_matrix->data.f[6], m_matrix->data.f[7],
					m_matrix->data.f[8], m_matrix->data.f[9], m_matrix->data.f[10], m_matrix->data.f[11],
					m_matrix->data.f[12], m_matrix->data.f[13], m_matrix->data.f[14], m_matrix->data.f[15]
				);
			}

			// Members work in place
			Matrix4^ Scale( const f32 f ) { m_matrix->Scale(f); return this; }

			Matrix4^ Add( Matrix4^ rhs ) { m_matrix->Add(*rhs->m_matrix); return this; }
			Matrix4^ Sub( Matrix4^ rhs ) { m_matrix->Sub(*rhs->m_matrix); return this; }
			Matrix4^ Mul( Matrix4^ rhs ) { m_matrix->Mul(*rhs->m_matrix); return this; }
			Matrix4^ Transpose() { m_matrix->Transpose(); return this; }
			Matrix4^ Invert() { m_matrix->Invert(); return this; }

			Matrix4^ Stabelize() { m_matrix->Stabelize(); return this; }

			f32 GetElemByIndex(int i) { return m_matrix->data.f[i]; }

			// non Member
			static Matrix4^ Scale( Matrix4^ mtx, const f32 f ) { return (gcnew Matrix4(mtx))->Scale(f); }

			static Matrix4^ Add( Matrix4^ lhs, Matrix4^ rhs ) { return (gcnew Matrix4(lhs))->Add(rhs); }
			static Matrix4^ Sub( Matrix4^ lhs, Matrix4^ rhs ) { return (gcnew Matrix4(lhs))->Sub(rhs); }
			static Matrix4^ Mul( Matrix4^ lhs, Matrix4^ rhs ) { return (gcnew Matrix4(lhs))->Mul(rhs); }
			static Matrix4^ Transpose( Matrix4^ mtx ) { return (gcnew Matrix4(mtx))->Transpose(); }
			static Matrix4^ Invert( Matrix4^ mtx ) { return (gcnew Matrix4(mtx))->Invert(); }

			static Matrix4^ RotateX( f32 angle ) { return gcnew Matrix4(ibMtx4::RotateX(angle)); }
			static Matrix4^ RotateY( f32 angle ) { return gcnew Matrix4(ibMtx4::RotateY(angle)); }
			static Matrix4^ RotateZ( f32 angle ) { return gcnew Matrix4(ibMtx4::RotateZ(angle)); }

			static Matrix4^ RotateEuler( f32 yaw, f32 pitch, f32 roll ) { return gcnew Matrix4(ibMtx4::RotateEuler(yaw, pitch, roll)); }
			static Matrix4^ RotateAxisAngle( Vector3^ axis, f32 angle );
			static Matrix4^ RotateAxisAngle( Vector4^ axis, f32 angle );
			static Matrix4^ RotateQuaternion( Quaternion^ quat );

			static void GetAxisAngle( Matrix4^ mtx, [Out] Vector3^% axis, [Out] f32% angle );
			static void GetAxisAngle( Matrix4^ mtx, [Out] Vector4^% axis, [Out] f32% angle );

			static Matrix4^ Stabelize( Matrix4^ mtx ) { return (gcnew Matrix4(mtx))->Stabelize(); }

			ibMtx4* m_matrix;

			static Matrix4^ operator+ ( Matrix4^ lhs, Matrix4^ rhs ) { return Matrix4::Add(lhs, rhs); }
			static Matrix4^ operator- ( Matrix4^ lhs, Matrix4^ rhs ) { return Matrix4::Sub(lhs, rhs); }
			static Matrix4^ operator* ( const f32 scale, Matrix4^ rhs ) { return Matrix4::Scale(rhs, scale); }
			static Matrix4^ operator* ( Matrix4^ lhs, const f32 scale ) { return Matrix4::Scale(lhs, scale); }
			static Matrix4^ operator* ( Matrix4^ lhs, Matrix4^ rhs ) { return Matrix4::Mul(lhs, rhs); }

			static Matrix4^ operator+= ( Matrix4^ lhs, Matrix4^ rhs ) { return lhs->Add(rhs); }
			static Matrix4^ operator-= ( Matrix4^ lhs, Matrix4^ rhs ) { return lhs->Sub(rhs); }
			static Matrix4^ operator*= ( Matrix4^ lhs, const f32 scale ) { return lhs->Scale(scale); }
			static Matrix4^ operator*= ( Matrix4^ lhs, Matrix4^ rhs ) { return lhs->Mul(rhs); }

			static bool operator== ( Matrix4^ lhs, Matrix4^ rhs ) { return *lhs->m_matrix == *rhs->m_matrix; }
			static bool operator!= ( Matrix4^ lhs, Matrix4^ rhs ) { return !(lhs == rhs); }
		};
	}
}
