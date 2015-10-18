#include <Windows.h>

extern "C" int GameMain();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lspzCmdLine, INT)
{
	return GameMain();
}
