#include <cfloat>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "ibMath.h"
#include "ibMtx2.h"
#include "ibVec2.h"

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

std::ostream& operator<<(std::ostream& o, const ibVec2& rhs)
{
	ibVec2 s = ibVec2::Stabelize(rhs);
	o << "[" << s.x << ", " << s.y << "]";
	return o;
}

void WriteHeader(std::ostream& o, const std::string& s)
{
	o << "\\\\\n\\\\ " << s << "\n\\\\" << std::endl;
}

void TestConstructors()
{
	WriteHeader(fout, "Constructors");
	ibVec2 uninit;
	ibVec2 rand = RandVec2();
	ibVec2 copy = rand;
	ibVec2 assign;
	assign = copy;

	fout << "Uninit: " << uninit << std::endl;
	fout << "Rand: " << rand << std::endl;
	fout << "Copy: " << copy << std::endl;
	fout << "Assign: " << assign << std::endl;
	fout.flush();
}

void TestMagNorm()
{
	WriteHeader(fout, "Magnitude and Normalize");
	ibVec2 v(3, 4);
	fout << "V: " << v;
	fout << "|V| = " << v.Magnitude() << ", |V|^2 = " << v.MagnitudeS() << std::endl;
	ibVec2 v2;
	v.NormalizeCopy(v2);
	fout << "Normalized: " << v.Normalize() << std::endl;
	fout << "Norm copy:  " << v2 << std::endl;
	fout.flush();
}

void TestMembers()
{
	WriteHeader(fout, "Members");
	ibVec2 a(2, 3), b(4, 5);
	fout << "A: " << a << std::endl;
	fout << "B: " << b << std::endl;
	fout << "A + B: " << a.Add(b) << std::endl;
	a.Sub(b);
	fout << "A - B: " << a.Sub(b) << std::endl;
	a.Add(b);
	fout << "2A: " << a.Mul(2) << std::endl;
	a.Mul(.5f);
	
	ibMtx2 M(2, 3, 2, 3);
	fout << "M:\n" << M;
	fout << "A * M: " << a.Mul(M) << std::endl;

	// reset
	a = ibVec2(2, 3);
	b = ibVec2(4, 5);
	fout << "A * B: " << a.Dot(b) << std::endl;
}

void TestNonMembers()
{
	WriteHeader(fout, "Non-members");
	ibVec2 a(2, 3), b(4, 5);
	ibMtx2 M(2, 3, 2, 3);
	fout << "A + B: " << ibVec2::Add(a, b) << std::endl;
	fout << "A - B: " << ibVec2::Sub(a, b) << std::endl;
	fout << "2A: " << ibVec2::Mul(a, 2) << std::endl;
	fout << "A * M: " << ibVec2::Mul(a, M) << std::endl;
	fout << "A * B: " << ibVec2::Dot(a, b) << std::endl;
}

void TestOperators()
{
	WriteHeader(fout, "Non-assignement Operators");
	ibVec2 a(2, 3), b(4, 5);
	ibMtx2 M(2, 3, 2, 3);
	fout << "A + B: " << a + b << std::endl;
	fout << "A - B: " << a - b << std::endl;
	fout << "A * B: " << a * b << std::endl;
	fout << "2 * A: " << 2 * a << std::endl;
	fout << "A * 2: " << a * 2 << std::endl;
	fout << "A * M: " << a * M << std::endl;

	WriteHeader(fout, "Assignment operators");
	fout << "A += B: " << (a += b) << std::endl;
	a -= b;
	fout << "A -= B: " << (a -= b) << std::endl;
	a += b;
	fout << "A *= 2: " << (a *= 2) << std::endl;
	a *= .5f;
	fout << "A *= M: " << (a *= M) << std::endl;

	WriteHeader(fout, "Comparison operators");
	fout << "A == A: " << (a == a) << std::endl;
	fout << "A == B: " << (a == b) << std::endl;
	fout << "A != A: " << (a != a) << std::endl;
	fout << "A != B: " << (a != b) << std::endl;
	fout.flush();
}

extern "C"
int GameMain()
{
	s_rng.seed(1729);

	TestConstructors();
	TestMagNorm();
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
