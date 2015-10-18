#include "test.h"

extern "C"
int GameMain()
{
	testfunc();

	TestClass tc;
	tc.Test();

	return 0;
}