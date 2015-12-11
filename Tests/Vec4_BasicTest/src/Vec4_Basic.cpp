#include <cfloat>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "ibMath.h"
#include "ibMtx2.h"
#include "ibMtx3.h"
#include "ibMtx4.h"
#include "ibVec2.h"
#include "ibVec3.h"
#include "ibVec4.h"

#include <Windows.h>

std::ofstream fout = std::ofstream("out.txt");
std::mt19937 s_rng;

f32 rand_flt(f32 min = 0.f, f32 max = 1.f)
{
	std::uniform_real_distribution<f32> ud(min, max);
	return ud(s_rng);
}

ibMtx2 RandMtx2()
{
	return ibMtx2(rand_flt(), rand_flt(), rand_flt(), rand_flt());
}

ibMtx3 RandMtx3()
{
	return ibMtx3(rand_flt(), rand_flt(), rand_flt(),
		          rand_flt(), rand_flt(), rand_flt(),
				  rand_flt(), rand_flt(), rand_flt());
}

ibMtx4 RandMtx4()
{
	return ibMtx4(rand_flt(), rand_flt(), rand_flt(), rand_flt(),
		          rand_flt(), rand_flt(), rand_flt(), rand_flt(),
				  rand_flt(), rand_flt(), rand_flt(), rand_flt(),
				  rand_flt(), rand_flt(), rand_flt(), rand_flt());
}

ibVec2 RandVec2()
{
	return ibVec2(rand_flt(), rand_flt());
}

std::ostream& operator<<(std::ostream& o, const ibMtx2& rhs)
{
	ibMtx2 stab = ibMtx2::Stabelize(rhs);
	o << "[ [ " << rhs.data.mtx._00 << ", " << rhs.data.mtx._01 << " ]\n";
	o << "  [ " << rhs.data.mtx._10 << ", " << rhs.data.mtx._11 << " ] ]" << std::endl;
	return o;
}

std::ostream& operator<<(std::ostream& o, const ibMtx3& rhs)
{
	ibMtx3 stab = ibMtx3::Stabelize(rhs);
	for (u32 i = 0; i < 3; ++i)
	{
		o << "[ ";
		for (u32 j = 0; j < 2; ++j)
			o << stab.data.a[i][j] << ", ";
		o << stab.data.a[i][2] << " ]\n";
	}
	return o;
}

std::ostream& operator<<(std::ostream& o, const ibMtx4& rhs)
{
	ibMtx4 stab = ibMtx4::Stabelize(rhs);
	for (u32 i = 0; i < 4; ++i)
	{
		o << "[ ";
		for (u32 j = 0; j < 3; ++j)
			o << stab.data.a[i][j] << ", ";
		o << stab.data.a[i][3] << " ]\n";
	}
	return o;
}

std::ostream& operator<<(std::ostream& o, const ibVec2& rhs)
{
	ibVec2 s = ibVec2::Stabelize(rhs);
	o << "[" << s.x << ", " << s.y << "]";
	return o;
}

std::ostream& operator<<(std::ostream& o, const ibVec3& rhs)
{
	ibVec3 s = ibVec3::Stabelize(rhs);
	o << "[" << s.x << ", " << s.y << ", " << s.z << "]";
	return o;
}

std::ostream& operator<<(std::ostream& o, const ibVec4& rhs)
{
	ibVec4 s = ibVec4::Stabelize(rhs);
	o << "[" << s.x << ", " << s.y << ", " << s.z << ", " << s.w << "]";
	return o;
}

void WriteHeader(const std::string& s)
{
	fout << "\\\\\n\\\\ " << s << "\n\\\\" << std::endl;
}

void TestConstructors()
{
	WriteHeader("Constructors");
	ibVec4 uninit;
	ibVec4 init(1,2,3,4);
	ibVec4 copy(init);
	ibVec4 assign;
	assign = init;

	fout << "Uninit: " << uninit << std::endl;
	fout << "Init: " << init << std::endl;
	fout << "Copy: " << copy << std::endl;
	fout << "Assign: " << assign << std::endl;
}

void TestMagnAndNorm()
{
	WriteHeader("Magnitude and normalize");
	ibVec4 a(4, 2, 3, 1);
	fout << "A: " << a << std::endl;
	fout << "Magnitude: " << a.Magnitude() << ", Mag^2: " << a.MagnitudeS() << std::endl;
	fout << "Normalized: " << a.Normalize() << std::endl;
	a = ibVec4(4, 2, 3, 1);
	ibVec4 copy;
	a.NormalizeCopy(copy);
	fout << "Normalized copy: " << copy << std::endl;
	fout << "A: " << a << std::endl;
}

void TestMembers()
{
	WriteHeader("Members");
	ibVec4 a(4, 2, 3, 1), b(2,2,3,1);
	fout << "A: " << a << std::endl;
	fout << "B: " << b << std::endl;
	fout << "A + B: " << a.Add(b) << std::endl;
	a.Sub(b);
	fout << "A - B: " << a.Sub(b) << std::endl;
	a.Add(b);
	fout << "2A: " << a.Mul(2) << std::endl;
	a.Mul(.5f);
	fout << "A * B: " << a.Dot(b) << std::endl;
	ibMtx4 mtx( 2.0f, 0.0f, 1.0f, 1.0f,
		        1.0f,  .5f, 2.0f, 0.3f,
				0.0f, 2.0f, 0.5f, 0.0f,
				0.0f, 1.0f, 1.0f, 2.5f);
	a = ibVec4(4, 2, 3, 1);
	fout << "M: " << mtx << std::endl;
	fout << "A * M: " << a.Mul(mtx) << std::endl;
	fout.flush();
}

void TestNonMembers()
{
	WriteHeader("Non-members");
	ibVec4 a(4, 2, 3, 1), b(2,2,3,1);
	ibMtx4 mtx( 2.0f, 0.0f, 1.0f, 1.0f,
		        1.0f,  .5f, 2.0f, 0.3f,
				0.0f, 2.0f, 0.5f, 0.0f,
				0.0f, 1.0f, 1.0f, 2.5f);
	fout << "A + B: " << ibVec4::Add(a, b) << std::endl;
	fout << "A - B: " << ibVec4::Sub(a, b) << std::endl;
	fout << "2A: " << ibVec4::Mul(2, a) << std::endl;
	fout << "A * 2: " << ibVec4::Mul(a, 2) << std::endl;
	fout << "A * B: " << ibVec4::Dot(a, b) << std::endl;
	fout << "A * M: " << ibVec4::Mul(a, mtx) << std::endl;
}

void TestOperators()
{
	WriteHeader("Non-assignment operators");
	ibVec4 a(4, 2, 3, 1), b(2,2,3,1);
	ibMtx4 mtx( 2.0f, 0.0f, 1.0f, 1.0f,
		        1.0f,  .5f, 2.0f, 0.3f,
				0.0f, 2.0f, 0.5f, 0.0f,
				0.0f, 1.0f, 1.0f, 2.5f);
	fout << "A + B: " << a + b << std::endl;
	fout << "A - B: " << a - b << std::endl;
	fout << "A * 2: " << a * 2 << std::endl;
	fout << "2A: " << 2 * a << std::endl;
	fout << "A * B: " << a * b << std::endl;
	fout << "A * M: " << a * mtx << std::endl;

	WriteHeader("Assignment operators");
	fout << "A + B: " << (a += b) << std::endl;
	a -= b;
	fout << "A - B: " << (a -= b) << std::endl;
	a += b;
	fout << "A * 2: " << (a *= 2) << std::endl;
	a *= .5f;
	fout << "A * M: " << (a *= mtx) << std::endl;

	WriteHeader("Comparison operators");
	fout << "A == A: " << (a == a) << std::endl;
	fout << "A != A: " << (a != a) << std::endl;
	fout << "A == B: " << (a == b) << std::endl;
	fout << "A != B: " << (a != b) << std::endl;
}

extern "C"
int GameMain()
{
	s_rng.seed(1729);

	TestConstructors();
	TestMagnAndNorm();
	TestMembers();
	TestNonMembers();
	TestOperators();

	Sleep(1000);
	return 0;
}

extern "C"
int GameUpdate(float)
{
	return 0;
}
