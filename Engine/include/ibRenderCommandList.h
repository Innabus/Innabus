#ifndef IB_RENDER_COMMAND_LIST_H
#define IB_RENDER_COMMAND_LIST_H
#pragma once

#include "ibRenderDevice.h"

// Please sync interface and comments about impl details with ibRenderDevice

class IB_RENDER_EXPORT ibRenderCommandList
{
public:
	// Creation without params requires Init
	ibRenderCommandList() {}
	ibRenderCommandList(u32 sizeBytes);
	ibRenderCommandList(u8* pBlock, u32 sizeBytes);
	~ibRenderCommandList();

	// Init takes ownership of pBlock
	void Init(u8* pBlock, u32 sizeBytes);

	void Splice(const ibRenderCommandList& cl);

	// Instead of using the globabl render device, allow command lists to execute onto
	// alternative devices; this is an extensibility thing in case we extend render
	// execution onto deferred render devices
	void Execute(ibRenderDevice* pExecuteDevice);

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Render commands (deferred, until Execute)
	///////////////////////////////////////////////////////////////////////////////////////////////
	void Clear(const float color[4], float depth, u8 stencil, bool clearState = false);
	void Present();

	// Blend state
	void BlendReset();
	void BlendAlphaToCoverage(bool bEnable);
	void BlendIndependent(bool bEnable);
	void BlendSetDesc(u32 renderTarget,
		              bool bEnable = false,
					  ibBlendFactor src = kBlend_One,
					  ibBlendFactor dst = kBlend_Zero,
					  ibBlendOp op = kBlendOp_Add,
					  ibBlendFactor srcAlpha = kBlend_One,
					  ibBlendFactor dstAlpha = kBlend_Zero,
					  ibBlendOp opAlpha = kBlendOp_Add,
					  u8 mask = kWriteEnable_All);
	void BlendCommit();

	// Depth stencil state
	void DepthStencilReset();
	void DepthEnable(bool bEnable);
	void DepthFunction(ibDepthFunction func);
	void StencilEnable(bool bEnable, u8 readMask = 0xFF, u8 writeMask = 0xFF);
	void StencilFrontFace(ibStencilOp fail, ibStencilOp depth, ibStencilOp pass, ibDepthFunction func);
	void StencilBackFace(ibStencilOp fail, ibStencilOp depth, ibStencilOp pass, ibDepthFunction func);
	void StencilSetReference(u32 refValue);
	void DepthStencilCommit();

	// Rasterizer state
	void RasterReset();
	void RasterWireframe(bool bWireframe);
	void RasterCull(ibCullMode mode, bool ccw = false);
	void RasterDepthBias(s32 bias, f32 clamp, f32 slopeBias);
	void RasterDepthClip(bool bClip);
	void RasterScissorEnable(bool bEnable);
	void RasterMultisample(bool bEnable);
	void RasterLineAA(bool bEnable);
	void RasterCommit();

	// Input assembler -- The input layout is bound by the vertex shader
	// Vertices are offset by index, the conversion to bytes is handled internally
	// by multiplying by the buffer's internal stride.
	void SetVertexBuffer(u32 slot, ibRenderVertexBuffer* pBuffer, u32 offset = 0);
	// Indices are offset by indice, ie. an offset of 2 references
	// the index at buffer[2].  The conversion to byte offset is 
	// handled internally
	void SetIndexBuffer(ibRenderIndexBuffer* pBuffer, u32 offset = 0);
	void SetTopology(ibTopology topology);

	// Vertex shader
	void SetVertexShaderConstants(u32 slot, ibRenderConstantBuffer* pBuffer);
	void SetVertexShaderSampler(u32 slot, ibRenderSampler* pSampler);
	void SetVertexShaderResource(u32 slot, ibRenderTexture* pTexture);
	void SetVertexShader(ibRenderShader& pShader);

	// Pixel shader
	void SetPixelShaderConstants(u32 slot, ibRenderConstantBuffer* pBuffer);
	void SetPixelShaderSampler(u32 slot, ibRenderSampler* pSampler);
	void SetPixelShaderResource(u32 slot, ibRenderTexture* pTexture);
	void SetPixelShader(ibRenderShader& pShader);

	// Rasterizer stage
	//void SetViewports(u32 count, ibRenderViewport* pViewports);
	//void SetScissors(u32 count, ibRenderScissor* pScissors);

	// Render targets (output merger)
	void SetRenderTargets(ibRenderTarget* pTargets, ibRenderDepthStencil* pDepthStencil, u32 numViews = 0);

	// Batch commit
	void Draw(u32 vertexCount, u32 vertexStart = 0);
	void DrawIndexed(u32 indexCount, u32 start, int vertexOffset = 0);

private:
	void ClearCommands();

	u32 m_size;
	u8* m_pCommands;
	u8* m_pEnd;
};

#endif // IB_RENDER_COMMAND_LIST_H
