#pragma once

using namespace System;

#include "ibVec3.h"
#include "Matrix3.cs.h"

namespace Innabus {
	namespace Math
	{
		public ref class Vector3
		{
		public:
			Vector3() { m_vector = new ibVec3; }; // No default init!
			Vector3( const f32 _x, const f32 _y, const f32 _z ) { m_vector = new ibVec3(_x, _y, _z); }
			Vector3( const ibVec3& rhs ) { m_vector = new ibVec3(rhs); }
			Vector3( Vector3% rhs ) { m_vector = new ibVec3(*rhs.m_vector); }
			Vector3( Vector3^ rhs ) { m_vector = new ibVec3(*rhs->m_vector); }
			Vector3% operator= ( Vector3% rhs ) { *m_vector = *rhs.m_vector; return *this; }

			~Vector3() { this->!Vector3(); }
			!Vector3() { delete m_vector; }

			f32 Magnitude() { return m_vector->Magnitude(); }
			f32 MagnitudeS() { return m_vector->MagnitudeS(); }

			Vector3^ Normalize() { m_vector->Normalize(); return this; }
			Vector3^ NormalizeCopy( Vector3^ to ) { m_vector->NormalizeCopy(*to->m_vector); return to; }

			// Member versions work "in place"
			Vector3^ Add( Vector3^ rhs ) { m_vector->Add(*rhs->m_vector); return this; }
			Vector3^ Sub( Vector3^ rhs ) { m_vector->Sub(*rhs->m_vector); return this; }
			Vector3^ Mul( const f32 rhs ) { m_vector->Mul(rhs); return this; }
			Vector3^ Mul( Matrix3^ rhs) { m_vector->Mul(*rhs->m_matrix); return this; }
			f32 Dot( Vector3^ rhs ) { return m_vector->Dot(*rhs->m_vector); }
			Vector3^ Cross( Vector3^ rhs ) { m_vector->Cross(*rhs->m_vector); return this; }

			Vector3^ Stabelize() { m_vector->Stabelize(); return this; }

			// Non-member
			static Vector3^ Add( Vector3^ lhs, Vector3^ rhs ) { return (gcnew Vector3(lhs))->Add(rhs); }
			static Vector3^ Sub( Vector3^ lhs, Vector3^ rhs ) { return (gcnew Vector3(lhs))->Sub(rhs); }
			static Vector3^ Mul( const f32 lhs, Vector3^ rhs ) { return (gcnew Vector3(rhs))->Mul(lhs); }
			static Vector3^ Mul( Vector3^ lhs, const f32 rhs ) { return (gcnew Vector3(lhs))->Mul(rhs); }
			static Vector3^ Mul( Vector3^ lhs, Matrix3^ rhs ) { return (gcnew Vector3(lhs))->Mul(rhs); }
			static f32 Dot( Vector3^ lhs, Vector3^ rhs ) { return (gcnew Vector3(lhs))->Dot(rhs); }
			static Vector3^ Cross( Vector3^ lhs, Vector3^ rhs ) { return (gcnew Vector3(lhs))->Cross(rhs); }

			static Vector3^ Stabelize( Vector3^ v ) { return (gcnew Vector3(v))->Stabelize(); }

			virtual String^ ToString() override
			{
				return String::Format("[{0}, {1}, {2}]", x, y, z);
			}

			property float x {
				float get() { return m_vector->x; }
				void set(float val) { m_vector->x = val; }
			}

			property float y {
				float get() { return m_vector->y; }
				void set(float val) { m_vector->y = val; }
			}

			property float z {
				float get() { return m_vector->z; }
				void set(float val) { m_vector->z = val; }
			}

			static Vector3^ operator+ ( Vector3^ lhs, Vector3^ rhs ) { return Vector3::Add(lhs, rhs); }
			static Vector3^ operator- ( Vector3^ lhs, Vector3^ rhs ) { return Vector3::Sub(lhs, rhs); }
			static Vector3^ operator* ( const f32 scale, Vector3^ rhs ) { return Vector3::Mul(rhs, scale); }
			static Vector3^ operator* ( Vector3^ lhs, const f32 scale ) { return Vector3::Mul(lhs, scale); }
			static f32 operator* ( Vector3^ lhs, Vector3^ rhs ) { return Vector3::Dot(lhs, rhs); }
			static Vector3^ operator* ( Vector3^ lhs, Matrix3^ rhs ) { return Vector3::Mul(lhs, rhs); }

			static Vector3^ operator+= ( Vector3^ lhs, Vector3^ rhs ) { return lhs->Add(rhs); }
			static Vector3^ operator-= ( Vector3^ lhs, Vector3^ rhs ) { return lhs->Sub(rhs); }
			static Vector3^ operator*= ( Vector3^ lhs, const f32 scale ) { return lhs->Mul(scale); }
			static f32 operator*= ( Vector3^ lhs, Vector3^ rhs ) { return lhs->Dot(rhs); }

			static bool operator==( Vector3^ lhs, Vector3^ rhs ) { return lhs->m_vector == rhs->m_vector; }
			static bool operator!=( Vector3^ lhs, Vector3^ rhs ) { return !(lhs == rhs); }

			ibVec3* m_vector;

		};
	}
}
