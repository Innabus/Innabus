#pragma once

using namespace System;

#include "ibVec4.h"
#include "Matrix4.cs.h"

namespace Innabus {
	namespace Math
	{
		public ref class Vector4
		{
		public:
			Vector4() { m_vector = new ibVec4; }; // No default init!
			Vector4( const f32 _x, const f32 _y, const f32 _z, const f32 _w ) { m_vector = new ibVec4(_x, _y, _z, _w); }
			Vector4( const ibVec4& rhs ) { m_vector = new ibVec4(rhs); }
			Vector4( Vector4% rhs ) { m_vector = new ibVec4(*rhs.m_vector); }
			Vector4( Vector4^ rhs ) { m_vector = new ibVec4(*rhs->m_vector); }
			Vector4% operator= ( Vector4% rhs ) { *m_vector = *rhs.m_vector; return *this; }

			~Vector4() { this->!Vector4(); }
			!Vector4() { delete m_vector; }

			f32 Magnitude() { return m_vector->Magnitude(); }
			f32 MagnitudeS() { return m_vector->MagnitudeS(); }

			Vector4^ Normalize() { m_vector->Normalize(); return this; }
			Vector4^ NormalizeCopy( Vector4^ to ) { m_vector->NormalizeCopy(*to->m_vector); return to; }

			// Member versions work "in place"
			Vector4^ Add( Vector4^ rhs ) { m_vector->Add(*rhs->m_vector); return this; }
			Vector4^ Sub( Vector4^ rhs ) { m_vector->Sub(*rhs->m_vector); return this; }
			Vector4^ Mul( const f32 rhs ) { m_vector->Mul(rhs); return this; }
			Vector4^ Mul( Matrix4^ rhs) { m_vector->Mul(*rhs->m_matrix); return this; }
			f32 Dot( Vector4^ rhs ) { return m_vector->Dot(*rhs->m_vector); }

			Vector4^ Stabelize() { m_vector->Stabelize(); return this; }

			// Non-member
			static Vector4^ Add( Vector4^ lhs, Vector4^ rhs ) { return (gcnew Vector4(lhs))->Add(rhs); }
			static Vector4^ Sub( Vector4^ lhs, Vector4^ rhs ) { return (gcnew Vector4(lhs))->Sub(rhs); }
			static Vector4^ Mul( const f32 lhs, Vector4^ rhs ) { return (gcnew Vector4(rhs))->Mul(lhs); }
			static Vector4^ Mul( Vector4^ lhs, const f32 rhs ) { return (gcnew Vector4(lhs))->Mul(rhs); }
			static Vector4^ Mul( Vector4^ lhs, Matrix4^ rhs ) { return (gcnew Vector4(lhs))->Mul(rhs); }
			static f32 Dot( Vector4^ lhs, Vector4^ rhs ) { return (gcnew Vector4(lhs))->Dot(rhs); }

			static Vector4^ Stabelize( Vector4^ v ) { return (gcnew Vector4(v))->Stabelize(); }

			virtual String^ ToString() override
			{
				return String::Format("[{0}, {1}, {2}, {3}]", x, y, z, w);
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

			property float w {
				float get() { return m_vector->w; }
				void set(float val) { m_vector->z = val; }
			}

			static Vector4^ operator+ ( Vector4^ lhs, Vector4^ rhs ) { return Vector4::Add(lhs, rhs); }
			static Vector4^ operator- ( Vector4^ lhs, Vector4^ rhs ) { return Vector4::Sub(lhs, rhs); }
			static Vector4^ operator* ( const f32 scale, Vector4^ rhs ) { return Vector4::Mul(rhs, scale); }
			static Vector4^ operator* ( Vector4^ lhs, const f32 scale ) { return Vector4::Mul(lhs, scale); }
			static f32 operator* ( Vector4^ lhs, Vector4^ rhs ) { return Vector4::Dot(lhs, rhs); }
			static Vector4^ operator* ( Vector4^ lhs, Matrix4^ rhs ) { return Vector4::Mul(lhs, rhs); }

			static Vector4^ operator+= ( Vector4^ lhs, Vector4^ rhs ) { return lhs->Add(rhs); }
			static Vector4^ operator-= ( Vector4^ lhs, Vector4^ rhs ) { return lhs->Sub(rhs); }
			static Vector4^ operator*= ( Vector4^ lhs, const f32 scale ) { return lhs->Mul(scale); }
			static f32 operator*= ( Vector4^ lhs, Vector4^ rhs ) { return lhs->Dot(rhs); }

			static bool operator==( Vector4^ lhs, Vector4^ rhs ) { return *lhs->m_vector == *rhs->m_vector; }
			static bool operator!=( Vector4^ lhs, Vector4^ rhs ) { return !(lhs == rhs); }

			ibVec4* m_vector;
		};
	}
}
