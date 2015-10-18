#include <Windows.h>

#include "ibSystem.h"

extern "C" int GameMain();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lspzCmdLine, INT)
{
	StartupGameInfo sgi;
	sgi.gameMain = GameMain;
	SystemStartup(&sgi);

//	return GameMain();
}
