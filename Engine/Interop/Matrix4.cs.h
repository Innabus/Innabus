#pragma once

using namespace System;

#include "ibMtx4.h"

namespace Innabus {
	namespace Math
	{
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
		};
	}
}
