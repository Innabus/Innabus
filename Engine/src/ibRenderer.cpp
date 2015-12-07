#include "ibRenderer.h"

//#include "ibMCP.h"
//#include "ibGameThread.h"

#include "ibLog.h"

void ibRenderer::Init()
{
}

void ibRenderer::Update()
{
	ibLog("Render Update\n");

	delete [] new char[20];
	// Swap command lists
	// Release game thread
	//g_gameThread.Advance();

	// Execute commands
}
