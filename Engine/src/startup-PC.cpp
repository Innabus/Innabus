#include <Windows.h>

#include "ibSystem.h"
#include "ibRenderer.h"

extern "C" int GameMain();
extern "C" int GameUpdate(float);

int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE, LPSTR /*lspzCmdLine*/, INT)
{
	StartupGameInfo sgi = { 0 };
	sgi.gameInit = GameMain;
	sgi.gameUpdate = GameUpdate;
	sgi.renderInit = &ibRenderer::Init;
	sgi.renderUpdate = &ibRenderer::Update;
	sgi.renderShutdown = &ibRenderer::Shutdown;

	SystemStartup(&sgi);
	return 0;
}
