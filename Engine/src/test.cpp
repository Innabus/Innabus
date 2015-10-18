#include <Windows.h>
#include <test.h>

void __declspec(dllexport) testfunc()
{
	MessageBoxA(NULL, "Test", "Test", MB_OK);
}

void TestClass::Test()
{
	MessageBoxA(NULL, "Class Test", "Class Test", MB_OK);
}
