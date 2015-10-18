#include <Windows.h>
#include "test.h"

extern "C"
int __declspec(dllexport) GameMain()
{
	testfunc();
	return 0;
}