#pragma once

using namespace System;

#include "ibQuat.h"
#include "Matrix4.cs.h"

namespace Innabus {
	namespace Math
	{
		public ref class Quaternion
		{
		public:
			Quaternion() { m_quat = new ibQuat; }
			Quaternion( const f32 _w, const f32 _x, const f32 _y, const f32 _z ) {
				m_quat = new ibQuat(_w, _x, _y, _z); 
			}
			Quaternion( const f32 pitch, const f32 yaw, const f32 roll ) {
				m_quat = new ibQuat(pitch, yaw, roll);
			}
			Quaternion( const f32 angle, Vector3^ vec ) {
				m_quat = new ibQuat(angle, *vec->m_vector);
			}
			Quaternion( const ibQuat% rhs ) { m_quat = new ibQuat(rhs); }
			Quaternion( Quaternion% rhs ) { m_quat = new ibQuat(*rhs.m_quat); }
			Quaternion( Quaternion^ rhs ) { m_quat = new ibQuat(*rhs->m_quat); }
			Quaternion% operator= (Quaternion% rhs) { *m_quat = *rhs.m_quat; return *this; }
			~Quaternion() { this->!Quaternion(); }
			!Quaternion() { delete m_quat; }

			virtual String^ ToString() override
			{
				return String::Format("[{0}, {1}, {2}, {3}]", w, x, y, z);
			}

			f32 Magnitude() { return m_quat->Magnitude(); }
			f32 MagnitudeS() { return m_quat->MagnitudeS(); }
			Quaternion^ Normalize() { m_quat->Normalize(); return this; }
			Quaternion^ Mul( Quaternion^ rhs ) { m_quat->Mul(*rhs->m_quat); return this; }
			Quaternion^ Invert() { m_quat->Invert(); return this; }
			Quaternion^ GetConjugate() { return gcnew Quaternion(m_quat->GetConjugate()); }
			Quaternion^ GetInverse() { return gcnew Quaternion(m_quat->GetInverse()); }
			Matrix4^ GetMatrix() { return gcnew Matrix4(m_quat->GetMatrix()); }

			void ToAxisAngle( Vector3^% axis, f32% angle ) {
				ibVec3 v;
				f32 f;
				m_quat->ToAxisAngle(&v, &f);
				angle = f;
				axis = gcnew Vector3(v);
			}

			Vector3^ RotatePt( Vector3^ pt ) { return gcnew Vector3(m_quat->RotatePt(*pt->m_vector)); }

			property float x {
				float get() { return m_quat->x; }
				void set(float val) { m_quat->x = val; }
			}

			property float y {
				float get() { return m_quat->y; }
				void set(float val) { m_quat->y = val; }
			}

			property float z {
				float get() { return m_quat->z; }
				void set(float val) { m_quat->z = val; }
			}

			property float w {
				float get() { return m_quat->w; }
				void set(float val) { m_quat->z = val; }
			}

			static Quaternion^ Lerp( Quaternion^ start, Quaternion^ end, f32 alpha ) {
				return gcnew Quaternion(ibQuat::Lerp(*start->m_quat, *end->m_quat, alpha));
			}
			static Quaternion^ Slerp( Quaternion^ start, Quaternion^ end, f32 alpha ) {
				return gcnew Quaternion(ibQuat::Slerp(*start->m_quat, *end->m_quat, alpha));
			}

			static Quaternion^ operator+ (Quaternion^ lhs, Quaternion^ rhs) {
				return gcnew Quaternion(*lhs->m_quat + *rhs->m_quat);
			}
			static Quaternion^ operator- (Quaternion^ lhs, Quaternion^ rhs) {
				return gcnew Quaternion(*lhs->m_quat - *rhs->m_quat);
			}
			static Quaternion^ operator* (Quaternion^ lhs, Quaternion^ rhs) {
				return gcnew Quaternion(*lhs->m_quat * *rhs->m_quat);
			}
			static Quaternion^ operator* (Quaternion^ lhs, Vector3^ rhs) {
				return gcnew Quaternion(*lhs->m_quat * *rhs->m_vector);
			}
			static Quaternion^ operator* (Quaternion^ lhs, f32 rhs) { 
				return gcnew Quaternion(*lhs->m_quat * rhs); 
			}
			static Quaternion^ operator~ (Quaternion^ q) { 
				return gcnew Quaternion(~(*q->m_quat)); 
			}

			static Quaternion^ operator+= (Quaternion^ lhs, Quaternion^ rhs) {
				*lhs->m_quat += *rhs->m_quat;
				return lhs;
			}
			static Quaternion^ operator-= (Quaternion^ lhs, Quaternion^ rhs) {
				*lhs->m_quat -= *rhs->m_quat;
				return lhs;
			}
			static Quaternion^ operator*= (Quaternion^ lhs, Quaternion^ rhs) {
				*lhs->m_quat *= *rhs->m_quat;
				return lhs;
			}
			static Quaternion^ operator*= (Quaternion^ lhs, f32 rhs) {
				*lhs->m_quat *= rhs;
				return lhs;
			}

			ibQuat* m_quat;
		};
	}
}
