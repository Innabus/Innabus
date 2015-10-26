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

std::ostream& operator<<(std::ostream& o, const ibMtx2& rhs)
{
	ibMtx2 stab = ibMtx2::Stabelize(rhs);
	o << "[ [ " << rhs.data.mtx._00 << ", " << rhs.data.mtx._01 << " ]\n";
	o << "  [ " << rhs.data.mtx._10 << ", " << rhs.data.mtx._11 << " ] ]" << std::endl;
	return o;
}

void WriteHeader(std::ostream& o, const std::string& s)
{
	o << "\\\\\n\\\\ " << s << "\n\\\\" << std::endl;
}

void TestConstructors()
{
	ibMtx2 uninit;
	ibMtx2 rand = RandMtx2();
	ibMtx2 copy(rand);
	ibMtx2 assign = rand;

	WriteHeader(fout, "Constructors");
	fout << "Uninitialized:\n" << uninit;
	fout << "Random:\n" << rand;
	fout << "Copy:\n" << copy;
	fout << "Assign:\n" << assign;
}

void TestMembers()
{
	WriteHeader(fout, "Member functions");
	ibMtx2 scaleBase = RandMtx2();
	f32 scale = rand_flt(1.f, 2.f);
	ibMtx2 scaled = scaleBase;
	scaled.Scale(scale);

	ibMtx2 a(2, 3, 4, 5), b(2, 3, 2, 3);
	// 1/Det(a) = ad-bc = 10 - 12 = -2
	fout << "A:\n" << a << "B:\n" << b;
	fout << "A + B:\n" << ibMtx2(a).Add(b);
	fout << "A - B:\n" << ibMtx2(a).Sub(b);
	fout << "A * B:\n" << ibMtx2(a).Mul(b); /// {{10,15},{18,27}}
	fout << "Transpose A:\n" << ibMtx2(a).Transpose();
	ibMtx2 inv = a;
	fout << "Invert A:\n" << inv.Invert();
	fout << "Verify:\n" << inv.Mul(a).Stabelize();
}

void TestNonMembers()
{
	WriteHeader(fout, "Non-member functions");
	ibMtx2 a(2, 3, 4, 5), b(2, 3, 2, 3);
	// 1/Det(a) = ad-bc = 10 - 12 = -2
	fout << "A:\n" << a << "B:\n" << b;
	fout << "A + B:\n" << ibMtx2::Add(a, b);
	fout << "A - B:\n" << ibMtx2::Sub(a, b);
	fout << "A * B:\n" << ibMtx2::Mul(a, b);
	fout << "Transpose A:\n" << ibMtx2::Transpose(a);
	fout << "Invert A:\n" << ibMtx2::Invert(a);
	fout << "Verify:\n" << ibMtx2::Stabelize(ibMtx2::Mul(ibMtx2::Invert(a), a));
}

void TestOperators()
{
	WriteHeader(fout, "Non-assignment operators");
	ibMtx2 a(2, 3, 4, 5), b(2, 3, 2, 3);
	fout << "A:\n" << a << "B:\n" << b;
	fout << "A + B:\n" << a + b;
	fout << "A - B:\n" << a - b;
	fout << "A * 2:\n" << a * 2;
	fout << "2 * A:\n" << 2 * a;
	fout << "A * B:\n" << a * b;

	WriteHeader(fout, "Assignment operators");
	fout << "A:\n" << a << "B:\n" << b;
	fout << "A += B:\n" << (a += b);
	a -= b;
	fout << "A -= B:\n" << (a -= b);
	a += b;
	fout << "A *= 2:\n" << (a *= 2);
	a *= .5f;
	fout << "A *= B:\n" << (a *= b);
}

void TestRotation()
{
	WriteHeader(fout, "Test rotation");

	ibMtx2 rot = ibMtx2::Rotation(deg_to_rads(30));
	ibVec2 v2(3, 0);
	fout << "Rotation matrux (30 deg):\n" << rot;
	fout << "Vector: [" << v2.x << ", " << v2.y << "]\n";
	ibVec2 result = v2 * rot;
	fout << "Result: [" << result.x << ", " << result.y << "]\n";
	fout << "Inverse:\n" << rot.Invert();
	result *= rot;
	fout << "Result: [" << result.x << ", " << result.y << "]" << std::endl;
	fout.flush();
}

extern "C"
int GameMain()
{
	s_rng.seed(1729);

	TestConstructors();
	TestMembers();
	TestNonMembers();
	TestOperators();

	TestRotation();

	fout.flush();

	Sleep(1000);
	return 0;
}
