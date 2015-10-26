#include <fstream>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>

#include "ibVec3.h"

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

std::ostream& operator<<(std::ostream& o, const ibVec3& v)
{
	o << "{x: " << v.x << " y: " << v.y << " z: " << v.z << "}";
	return o;
}

void WriteHeader(std::ostream& o, const std::string& s)
{
	o << "\\\\\n\\\\ " << s << "\n\\\\" << std::endl;
}

void WriteV3WithMagnitudes(std::ostream& o, const ibVec3& v)
{
	o << v << " Magnitude: " << v.Magnitude() << " (^2): " << v.MagnitudeS() << std::endl;
}

void WriteMagnitudes(std::ostream& o, std::vector<ibVec3>& v)
{
	for(const auto& a : v)
		WriteV3WithMagnitudes(o, a);
}

void MemberArithmetic(std::ofstream& fout, std::vector<ibVec3> v)
{
	WriteHeader(fout, "Member arithmetic");
	fout << v[2] << " + " << v[0] << " = ";
	v[2].Add(v[1]);
	fout << v[2] << std::endl;
	fout << v[2] << " - " << v[1] << " = ";
	v[2].Sub(v[1]);
	fout << v[2] << std::endl;
	fout << v[2] << " * 0.5 = ";
	fout << (v[2].Mul(0.5f)) << std::endl;

	ibVec3 a(2, 3, 4), b(5, 6, 7);
	fout << "Dot: " << a << b << a.Dot(b) << std::endl;

	fout << "Cross: " << a << "x" << b << " = ";
	fout << a.Cross(b) << std::endl;
}

void NonmemberArithetic(std::ofstream& fout, std::vector<ibVec3> v)
{
	WriteHeader(fout, "Nonmember arithmetic");
	fout << v[2] << " + " << v[0] << " = " << ibVec3::Add(v[2], v[0]) << std::endl;
	v[2].Add(v[1]);
	fout << v[2] << " - " << v[1] << " = " << ibVec3::Sub(v[2], v[1]) << std::endl;
	v[2].Sub(v[1]);
	fout << v[2] << " * 0.5 = " << ibVec3::Mul(0.5f, v[2]) << std::endl;
	fout << "\t(" << ibVec3::Mul(v[2], 0.5f) << ")" << std::endl;

	ibVec3 a(2, 3, 4), b(5, 6, 7);
	fout << "Dot: " << a << b << ibVec3::Dot(a, b) << std::endl;

	fout << "Cross: " << a << "x" << b << " = " << ibVec3::Cross(a, b) << std::endl;
}

void OperatorArithmetic(std::ofstream& fout, std::vector<ibVec3> v)
{
	WriteHeader(fout, "Operator arithmetic");
	fout << v[2] << " + " << v[0] << " = " << v[2] + v[0] << std::endl;
	v[2].Add(v[1]);
	fout << v[2] << " - " << v[1] << " = " << v[2] - v[1] << std::endl;
	v[2].Sub(v[1]);
	fout << v[2] << " * 0.5 = " << v[2] * 0.5f << std::endl;
	fout << "\t(" << 0.5f * v[2] << ")" << std::endl;

	ibVec3 a(2, 3, 4), b(5, 6, 7);
	fout << "Dot: " << a << b << a * b << std::endl;
}

void AssignmentOperatorArithmetic(std::ofstream& fout, std::vector<ibVec3> v)
{
	WriteHeader(fout, "Assignment operator arithmetic");
	fout << v[2] << " + " << v[0] << " = ";
	fout << (v[2] += v[0]) << std::endl;
	fout << v[2] << " - " << v[1] << " = ";
	fout << (v[2] -= v[1]) << std::endl;
	fout << v[2] << " *= 0.5 = ";
	fout << (v[2] *= 0.5f) << std::endl;
}

extern "C"
int GameMain()
{
	std::ofstream fout = std::ofstream("out.txt");
	s_rng.seed(std::random_device()());

	std::vector<ibVec3> v(3);

	// Test magnitude
	WriteHeader(fout, "Magnitude");
	v[0] = RandVector();
	v[1] = RandVector();
	v[2] = ibVec3(3, 4, 5);

	WriteMagnitudes(fout, v);

	// Test Normalize
	WriteHeader(fout, "Normalize");
	for (auto& a: v)
		a.Normalize();
	WriteMagnitudes(fout, v);

	// Arithmetic
	v[0] = v[1] = RandVector();
	v[2] = ibVec3(1, 2, 3);
	MemberArithmetic(fout, v);
	NonmemberArithetic(fout, v);
	OperatorArithmetic(fout, v);
	AssignmentOperatorArithmetic(fout, v);

	WriteHeader(fout, "Static members");
	fout << ibVec3::ZERO << std::endl;
	fout << ibVec3::ONE << std::endl;
	fout << ibVec3::X_AXIS << std::endl;
	fout << ibVec3::Y_AXIS << std::endl;
	fout << ibVec3::Z_AXIS << std::endl;

	return 0;
}
