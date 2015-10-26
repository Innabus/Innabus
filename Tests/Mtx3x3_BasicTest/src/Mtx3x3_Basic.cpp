#include <cfloat>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "ibMath.h"
#include "ibMtx2.h"
#include "ibMtx3.h"
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

ibMtx3 RandMtx3()
{
	return ibMtx3(rand_flt(), rand_flt(), rand_flt(),
		          rand_flt(), rand_flt(), rand_flt(),
				  rand_flt(), rand_flt(), rand_flt());
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
	ibMtx3 uninit;
	ibMtx3 init( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	ibMtx3 copy(init);
	ibMtx3 assign;
	assign = copy;

	fout << "Uninit:\n" << uninit;
	fout << "Init:\n" << init;
	fout << "Copy:\n" << copy;
	fout << "Assign:\n" << assign;
	fout.flush();
}

void TestMembers()
{
	WriteHeader(fout, "Members");
	ibMtx3 a( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	ibMtx3 b( 3, 2, 1, 2, 1, 3, 1, 3, 2 );

	fout << "A:\n" << a;
	fout << "B:\n" << b;
	fout << "A + B:\n" << a.Add(b);
	a = ibMtx3( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	fout << "A - B:\n" << a.Sub(b);
	a = ibMtx3( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	fout << "A * B:\n" << a.Mul(b);
	a = ibMtx3( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	fout << "Transpose A:\n" << a.Transpose();
	a.Transpose();
	ibMtx3 inv = a;
	fout << "Inverse A:\n" << inv.Invert();
	fout << "Verify:\n" << inv.Mul(a);
	fout.flush();
}

void TestNonMembers()
{
	WriteHeader(fout, "Non-members");
	ibMtx3 a( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	ibMtx3 b( 3, 2, 1, 2, 1, 3, 1, 3, 2 );

	fout << "A + B:\n" << ibMtx3::Add(a, b);
	fout << "A - B:\n" << ibMtx3::Sub(a, b);
	fout << "A * B:\n" << ibMtx3::Mul(a, b);
	fout << "Transpose A:\n" << ibMtx3::Transpose(a);
	fout << "Inverse A:\n" << ibMtx3::Invert(a);
	fout << "Verify:\n" << ibMtx3::Mul(a, ibMtx3::Invert(a));
	fout.flush();
}

void TestOperators()
{
	WriteHeader(fout, "Non-assignment operators");
	ibMtx3 a( 1, 2, 3, 3, 2, 1, 2, 1, 3 );
	ibMtx3 b( 3, 2, 1, 2, 1, 3, 1, 3, 2 );
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

extern "C"
int GameMain()
{
	s_rng.seed(1729);

	TestConstructors();
	TestMembers();
	TestNonMembers();

	Sleep(1000);
	return 0;
}
