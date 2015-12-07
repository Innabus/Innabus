#include <Windows.h>

#include "ibSystem.h"
#include "ibRenderer.h"

extern "C" int GameMain();

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE, LPSTR /*lspzCmdLine*/, INT)
{
	StartupGameInfo sgi = { 0 };
	sgi.gameInit = GameMain;
	sgi.renderInit = &ibRenderer::Init;
	sgi.renderUpdate = &ibRenderer::Update;

	SystemStartup(&sgi);
}
