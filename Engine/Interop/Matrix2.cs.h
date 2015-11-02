#pragma once

using namespace System;

#include "ibMtx2.h"

namespace Innabus {
	namespace Math
	{
		public ref class Matrix2
		{
		public:
			Matrix2() { m_matrix = new ibMtx2(); } // No default initialization
			Matrix2( f32 a, f32 b, f32 c, f32 d ) { m_matrix = new ibMtx2(a, b, c, d); }
			Matrix2( const ibMtx2& mtx ) { m_matrix = new ibMtx2(mtx); }
			Matrix2( Matrix2% rhs ) { m_matrix = new ibMtx2(*rhs.m_matrix); }
			Matrix2( Matrix2^ rhs ) { m_matrix = new ibMtx2(*rhs->m_matrix); }
			Matrix2% operator= ( Matrix2% rhs ) { *m_matrix = *rhs.m_matrix; return *this; }

			~Matrix2() { this->!Matrix2(); }
			!Matrix2() { delete m_matrix; }

			// Members work in place
			Matrix2^ Scale( const f32 f ) { m_matrix->Scale(f); return this; }

			Matrix2^ Add( Matrix2^ rhs ) { m_matrix->Add(*rhs->m_matrix); return this; }
			Matrix2^ Sub( Matrix2^ rhs ) { m_matrix->Sub(*rhs->m_matrix); return this; }
			Matrix2^ Mul( Matrix2^ rhs ) { m_matrix->Mul(*rhs->m_matrix); return this; }
			Matrix2^ Transpose() { m_matrix->Transpose(); return this; }
			Matrix2^ Invert() { m_matrix->Invert(); return this; }

			Matrix2^ Stabelize() { m_matrix->Stabelize(); return this; }

			f32 GetElemByIndex(int i) { return m_matrix->data.f[i]; }

			// non Member
			static Matrix2^ Scale( Matrix2^ mtx, const f32 f ) { return (gcnew Matrix2(mtx))->Scale(f); }

			static Matrix2^ Add( Matrix2^ lhs, Matrix2^ rhs ) { return (gcnew Matrix2(lhs))->Add(rhs); }
			static Matrix2^ Sub( Matrix2^ lhs, Matrix2^ rhs ) { return (gcnew Matrix2(lhs))->Sub(rhs); }
			static Matrix2^ Mul( Matrix2^ lhs, Matrix2^ rhs ) { return (gcnew Matrix2(lhs))->Mul(rhs); }
			static Matrix2^ Transpose( Matrix2^ mtx ) { return (gcnew Matrix2(mtx))->Transpose(); }
			static Matrix2^ Invert( Matrix2^ mtx ) { return (gcnew Matrix2(mtx))->Invert(); }

			static Matrix2^ Rotation( f32 angle ) { return gcnew Matrix2(ibMtx2::Rotation(angle)); }
			static Matrix2^ Stabelize( Matrix2^ mtx ) { return (gcnew Matrix2(mtx))->Stabelize(); }

			ibMtx2* m_matrix;

			static Matrix2^ operator+ ( Matrix2^ lhs, Matrix2^ rhs ) { return Matrix2::Add(lhs, rhs); }
			static Matrix2^ operator- ( Matrix2^ lhs, Matrix2^ rhs ) { return Matrix2::Sub(lhs, rhs); }
			static Matrix2^ operator* ( f32 scale, Matrix2^ rhs ) { return Matrix2::Scale(rhs, scale); }
			static Matrix2^ operator* ( Matrix2^ lhs, f32 scale ) { return Matrix2::Scale(lhs, scale); }
			static Matrix2^ operator* ( Matrix2^ lhs, Matrix2^ rhs ) { return Matrix2::Mul(lhs, rhs); }

			static Matrix2^ operator+= ( Matrix2^ lhs, Matrix2^ rhs ) { return lhs->Add(rhs); }
			static Matrix2^ operator-= ( Matrix2^ lhs, Matrix2^ rhs ) { return lhs->Sub(rhs); }
			static Matrix2^ operator*= ( Matrix2^ lhs, f32 scale ) { return lhs->Scale(scale); }
			static Matrix2^ operator*= ( Matrix2^ lhs, Matrix2^ rhs ) { return lhs->Mul(rhs); }
		};
	}
}
