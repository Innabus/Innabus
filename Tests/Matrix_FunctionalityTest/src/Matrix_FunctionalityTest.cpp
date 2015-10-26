#include <fstream>
#include <cfloat>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "ibVec3.h"
#include "ibMtx4.h"

std::mt19937 s_rng;

f32 rand_flt(f32 min = 0.f, f32 max = 1.f)
{
	std::uniform_real_distribution<f32> ud(min, max);
	return ud(s_rng);
}

ibVec3 RandVector()
{
	return ibVec3(rand_flt(), rand_flt(), rand_flt());
}

ibMtx4 RandMatrix()
{
	return ibMtx4(rand_flt(), rand_flt(), rand_flt(), rand_flt(),
		          rand_flt(), rand_flt(), rand_flt(), rand_flt(),
				  rand_flt(), rand_flt(), rand_flt(), rand_flt(),
				  rand_flt(), rand_flt(), rand_flt(), rand_flt());
}

std::ostream& operator<<(std::ostream& o, const ibVec3& v)
{
	o << "{x: " << v.x << " y: " << v.y << " z: " << v.z << "}";
	return o;
}

std::ostream& operator<<(std::ostream& o, const ibMtx4& m)
{
	o << "[ [ "<< m.data.mtx._00 << ", " << m.data.mtx._01 << ", " << m.data.mtx._02 << ", " << m.data.mtx._03 << " ]  \n";
	o << "  [ "<< m.data.mtx._10 << ", " << m.data.mtx._11 << ", " << m.data.mtx._12 << ", " << m.data.mtx._13 << " ]  \n";
	o << "  [ "<< m.data.mtx._20 << ", " << m.data.mtx._21 << ", " << m.data.mtx._22 << ", " << m.data.mtx._23 << " ]  \n";
	o << "  [ "<< m.data.mtx._30 << ", " << m.data.mtx._31 << ", " << m.data.mtx._32 << ", " << m.data.mtx._33 << " ] ]";
	o << std::endl;
	return o;
}

void WriteHeader(std::ostream& o, const std::string& s)
{
	o << "\\\\\n\\\\ " << s << "\n\\\\" << std::endl;
}

ibMtx4& StabelizeMatrix(ibMtx4& m)
{
	for (u32 i = 0; i < 4; ++i)
	{
		for (u32 j = 0; j < 4; ++j)
		{
			if (fabs(m.data.a[i][j]) <= FLT_EPSILON)
				m.data.a[i][j] = 0.f;
		}
	}
	return m;
}

void ScaleTranslateTest(std::ostream& o, ibMtx4 m)
{
	ibMtx4 copy = m;
	ibVec3 v = ibVec3(4, 5, -2);
	o << "Scale: 2.5f\n";
	o << copy.Scale(2.5f) << std::endl;
	o << "Translate:\n" << v << "\n" << m.Translate(v) << std::endl;
}

void ScaleTranslateCopyTest(std::ostream& o, ibMtx4 m)
{
	ibVec3 v = RandVector();
	o << "Scale: 2.5f\n";
	o << ibMtx4::Scale(m, 2.5f) << std::endl;
	o << "Translate:\n" << v << "\n" << ibMtx4::Translate(m, v) << std::endl;
}

void InPlaceArithmeticTest(std::ostream& o, ibMtx4 a, ibMtx4 b)
{
	ibMtx4 copy = a;
	ibMtx4 inv = a;
	o << "A + B\n" << a.Add(b) << "\n";
	a = copy;
	o << "A - B\n" << a.Sub(b) << "\n";
	a = copy;
	o << "A * B\n" << a.Mul(b) << "\n";
	o << "A^T\n" << copy.Transpose() << "\n";
	o << "A^-1\n" << inv.Invert() << "\n";
	copy.Transpose();
	o << "Verify\n" << StabelizeMatrix(copy.Mul(inv)) << "\n";
}

void CopyArithmeticTest(std::ostream& o, ibMtx4 a, ibMtx4 b)
{
	o << "A + B\n" << ibMtx4::Add(a, b) << "\n";
	o << "A - B\n" << ibMtx4::Sub(a, b) << "\n";
	o << "A * B\n" << ibMtx4::Mul(a, b) << "\n";
	o << "A^T\n" << ibMtx4::Transpose(a) << "\n";
	o << "A^-1\n" << ibMtx4::Invert(a) << "\n";
	ibMtx4 ainv = ibMtx4::Mul(a, ibMtx4::Invert(a));
	o << "Verify\n" << StabelizeMatrix(ainv) << "\n";
}

extern "C"
int GameMain()
{
	std::ofstream fout = std::ofstream("out.txt");
	s_rng.seed(1729);

	ibMtx4 id = ibMtx4::IDENTITY;
	fout << "Let ID = \n" << id;

	WriteHeader(fout, "Scale and translate on ID");
	ScaleTranslateTest(fout, id);
	WriteHeader(fout, "Scale and translate with ID");
	ScaleTranslateCopyTest(fout, id);

	ibMtx4 a = RandMatrix(), b = RandMatrix();
	fout << "Let A = \n" << a;
	fout << "Let B = \n" << b;
	WriteHeader(fout, "Matrix arithmetic (in place)");
	InPlaceArithmeticTest(fout, a, b);
	WriteHeader(fout, "Matrix arithmetic (copy)");
	CopyArithmeticTest(fout, a, b);

	return 0;
}
