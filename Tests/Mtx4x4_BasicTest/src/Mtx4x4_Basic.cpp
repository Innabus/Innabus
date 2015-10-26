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
	ibMtx4 uninit;
	ibMtx4 init( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	ibMtx4 copy(init);
	ibMtx4 assign;
	assign = copy;

	fout << "Uninit:\n" << uninit;
	fout << "Init:\n" << init;
	fout << "Copy:\n" << copy;
	fout << "Assign:\n" << assign;
	fout.flush();
}

void TestMembers()
{
	WriteHeader("Members");
	ibMtx4 a( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	ibMtx4 b( 1, 2, 3, 4, 0, 1, 1, 2, 1, 0, 2, 0, 0, 3, 2, 1 );

	fout << "A:\n" << a;
	fout << "B:\n" << b;
	fout << "A + B:\n" << a.Add(b);
	a = ibMtx4( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	fout << "A - B:\n" << a.Sub(b);
	a = ibMtx4( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	fout << "A * B:\n" << a.Mul(b);
	a = ibMtx4( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	fout << "Transpose A:\n" << a.Transpose();
	a.Transpose();
	ibMtx4 inv = a;
	fout << "Inverse A:\n" << inv.Invert();
	fout << "Verify:\n" << inv.Mul(a);
	fout.flush();
}

void TestNonMembers()
{
	WriteHeader("Non-members");
	ibMtx4 a( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	ibMtx4 b( 1, 2, 3, 4, 0, 1, 1, 2, 1, 0, 2, 0, 0, 3, 2, 1 );

	fout << "A + B:\n" << ibMtx4::Add(a, b);
	fout << "A - B:\n" << ibMtx4::Sub(a, b);
	fout << "A * B:\n" << ibMtx4::Mul(a, b);
	fout << "Transpose A:\n" << ibMtx4::Transpose(a);
	fout << "Inverse A:\n" << ibMtx4::Invert(a);
	fout << "Verify:\n" << ibMtx4::Mul(a, ibMtx4::Invert(a));
	fout.flush();
}

void TestOperators()
{
	WriteHeader("Non-assignment operators");
	ibMtx4 a( 4, 2, 3, 1, 1, 2, 2, 1, 0, .5, 3, 0, 1, 0, 2, 1 );
	ibMtx4 b( 1, 2, 3, 4, 0, 1, 1, 2, 1, 0, 2, 0, 0, 3, 2, 1 );
	fout << "A:\n" << a << "B:\n" << b;
	fout << "A + B:\n" << a + b;
	fout << "A - B:\n" << a - b;
	fout << "A * 2:\n" << a * 2;
	fout << "2 * A:\n" << 2 * a;
	fout << "A * B:\n" << a * b;

	WriteHeader("Assignment operators");
	fout << "A:\n" << a << "B:\n" << b;
	fout << "A += B:\n" << (a += b);
	a -= b;
	fout << "A -= B:\n" << (a -= b);
	a += b;
	fout << "A *= 2:\n" << (a *= 2);
	a *= .5f;
	fout << "A *= B:\n" << (a *= b);
}

extern "C"
int GameMain()
{
	s_rng.seed(1729);

	TestConstructors();
	TestMembers();
	TestNonMembers();
	TestOperators();

	Sleep(1000);
	return 0;
}
