#include <fstream>
#include <cstdlib>

#include "ibColor.h"
#include "ibRenderObject.h"
#include "ibRenderCommandList.h"
#include "ibRenderPasses.h"
#include "ibRenderConstantBuffer.h"
#include "ibRenderer.h"
#include "ibRenderDevice.h"
#include "ibRenderShaderManager.h"
#include "ibShaders.h"

class TestRenderObject : public ibRenderObject
{
public:
	void Render(ibRenderCommandList& commandList) {
		// Do rendery things
		if (!m_cbuf.isValid())
			m_cbuf = g_renderDevice.CreateConstantBuffer(sizeof(ibColorF));
		commandList.SetVertexShader(g_shaderManager.GetShader(g_shaders[kCommonShader_VsQuad]));
		commandList.SetTopology(kTopology_Triangle);
		commandList.SetVertexBuffer(0, 0);

		commandList.SetPixelShader(g_shaderManager.GetShader(g_shaders[kCommonShader_PsQuadSolid]));
		
		ibColorF color(1.f, 1.f, 1.f, 1.f);
		commandList.SetPixelShaderConstants(0, &m_cbuf);
		commandList.Draw(6);
	}

private:
	ibRenderConstantBuffer m_cbuf;

} g_testRenderObject;

extern "C"
int GameMain()
{
	std::ofstream fout = std::ofstream("out.txt");

	// Add render object to opaque pass
	g_opaquePass.RegisterRenderObject(&g_testRenderObject);

	return 0;
}

extern "C"
int GameUpdate(float)
{
	// Gmae logic update

	// Execute render passes
	g_renderPasses.Execute();

	return 1;
}
