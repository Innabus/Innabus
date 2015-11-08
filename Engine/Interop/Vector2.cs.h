#pragma once

using namespace System;

#include "ibVec2.h"
#include "Matrix2.cs.h"

namespace Innabus {
	namespace Math
	{
		public ref class Vector2
		{
		public:
			Vector2() { m_vector = new ibVec2; }; // No default init!
			Vector2( const f32 _x, const f32 _y ) { m_vector = new ibVec2(_x, _y); }
			Vector2( Vector2% rhs ) { m_vector = new ibVec2(*rhs.m_vector); }
			Vector2( Vector2^ rhs ) { m_vector = new ibVec2(*rhs->m_vector); }
			Vector2% operator= ( Vector2% rhs ) { *m_vector = *rhs.m_vector; return *this; }

			~Vector2() { this->!Vector2(); }
			!Vector2() { delete m_vector; }

			f32 Magnitude() { return m_vector->Magnitude(); }
			f32 MagnitudeS() { return m_vector->MagnitudeS(); }

			Vector2^ Normalize() { m_vector->Normalize(); return this; }
			Vector2^ NormalizeCopy( Vector2^ to ) { m_vector->NormalizeCopy(*to->m_vector); return to; }

			// Member versions work "in place"
			Vector2^ Add( Vector2^ rhs ) { m_vector->Add(*rhs->m_vector); return this; }
			Vector2^ Sub( Vector2^ rhs ) { m_vector->Sub(*rhs->m_vector); return this; }
			Vector2^ Mul( const f32 rhs ) { m_vector->Mul(rhs); return this; }
			Vector2^ Mul( Matrix2^ rhs) { m_vector->Mul(*rhs->m_matrix); return this; }
			f32 Dot( Vector2^ rhs ) { return m_vector->Dot(*rhs->m_vector); }

			Vector2^ Stabelize() { m_vector->Stabelize(); return this; }

			// Non-member
			static Vector2^ Add( Vector2^ lhs, Vector2^ rhs ) { return (gcnew Vector2(lhs))->Add(rhs); }
			static Vector2^ Sub( Vector2^ lhs, Vector2^ rhs ) { return (gcnew Vector2(lhs))->Sub(rhs); }
			static Vector2^ Mul( const f32 lhs, Vector2^ rhs ) { return (gcnew Vector2(rhs))->Mul(lhs); }
			static Vector2^ Mul( Vector2^ lhs, const f32 rhs ) { return (gcnew Vector2(lhs))->Mul(rhs); }
			static Vector2^ Mul( Vector2^ lhs, Matrix2^ rhs ) { return (gcnew Vector2(lhs))->Mul(rhs); }
			static f32 Dot( Vector2^ lhs, Vector2^ rhs ) { return (gcnew Vector2(lhs))->Dot(rhs); }

			static Vector2^ Stabelize( Vector2^ v ) { return (gcnew Vector2(v))->Stabelize(); }

			virtual String^ ToString() override
			{
				return String::Format("[{0}, {1}]", x, y );
			}

			property float x {
				float get() { return m_vector->x; }
				void set(float val) { m_vector->x = val; }
			}

			property float y {
				float get() { return m_vector->y; }
				void set(float val) { m_vector->y = val; }
			}

			static Vector2^ operator+ ( Vector2^ lhs, Vector2^ rhs ) { return Vector2::Add(lhs, rhs); }
			static Vector2^ operator- ( Vector2^ lhs, Vector2^ rhs ) { return Vector2::Sub(lhs, rhs); }
			static Vector2^ operator* ( const f32 scale, Vector2^ rhs ) { return Vector2::Mul(rhs, scale); }
			static Vector2^ operator* ( Vector2^ lhs, const f32 scale ) { return Vector2::Mul(lhs, scale); }
			static f32 operator* ( Vector2^ lhs, Vector2^ rhs ) { return Vector2::Dot(lhs, rhs); }
			static Vector2^ operator* ( Vector2^ lhs, Matrix2^ rhs ) { return Vector2::Mul(lhs, rhs); }

			static Vector2^ operator+= ( Vector2^ lhs, Vector2^ rhs ) { return lhs->Add(rhs); }
			static Vector2^ operator-= ( Vector2^ lhs, Vector2^ rhs ) { return lhs->Sub(rhs); }
			static Vector2^ operator*= ( Vector2^ lhs, const f32 scale ) { return lhs->Mul(scale); }
			static f32 operator*= ( Vector2^ lhs, Vector2^ rhs ) { return lhs->Dot(rhs); }

			static bool operator==( Vector2^ lhs, Vector2^ rhs ) { return *lhs->m_vector == *rhs->m_vector; }
			static bool operator!=( Vector2^ lhs, Vector2^ rhs ) { return !(lhs == rhs); }

			ibVec2* m_vector;

		};
	}
}
