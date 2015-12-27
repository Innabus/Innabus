#include "ibRenderCommandList.h"

#include "ibRenderCommands.h"

ibRenderCommandList::ibRenderCommandList(u32 sizeBytes):m_size(sizeBytes)
{
	Init(0, sizeBytes);
}

ibRenderCommandList::ibRenderCommandList(u8* pBlock, u32 sizeBytes):m_size(sizeBytes)
{
	Init(pBlock, sizeBytes);
}

ibRenderCommandList::~ibRenderCommandList()
{
	delete [] m_pCommands;
}

void ibRenderCommandList::Init(u8* pBlock, u32 sizeBytes)
{
	if (!pBlock)
		pBlock = new (g_engineHeap->Alloc(sizeBytes, "Render Command List")) u8[sizeBytes];

	m_pCommands = pBlock;
	m_pEnd = pBlock;
	m_size = sizeBytes;
}

void ibRenderCommandList::Splice(const ibRenderCommandList& cl)
{
	u32 dist = cl.m_pEnd - cl.m_pCommands;
	u32 capacity = m_size - (m_pEnd - m_pCommands);
	ibAssert(dist < capacity);

	memcpy(m_pEnd, cl.m_pCommands, dist);
	m_pEnd += dist;
}

void ibRenderCommandList::Execute(ibRenderDevice* pDevice)
{
	u8* pWorking = m_pCommands;
	while (pWorking < m_pEnd)
	{
		ibRenderCommand* pCommand = (ibRenderCommand*)pWorking;
		(*pCommand)(pDevice);
		pWorking += pCommand->size;
	}
	ibAssert(pWorking == m_pEnd);
	m_pEnd = m_pCommands;
}

#define AdjustEnd(x) pCmd->size += sizeof(x); m_pEnd += pCmd->size;

void ibRenderCommandList::Clear(const float color[4], float depth, u8 stencil, bool clearState /*= false*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_Clear(ibColorF(color[0], color[1], color[2], color[3]), depth, stencil, clearState);
	AdjustEnd(ibRenderCommand_Clear);
}

void ibRenderCommandList::Present()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_Present;
	AdjustEnd(ibRenderCommand_Present);
}

// Blend state
void ibRenderCommandList::BlendReset()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_BlendReset;
	AdjustEnd(ibRenderCommand_BlendReset);
}

void ibRenderCommandList::BlendAlphaToCoverage(bool bEnable)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_BlendAlphaToCoverage(bEnable);
	AdjustEnd(ibRenderCommand_BlendAlphaToCoverage);
}

void ibRenderCommandList::BlendIndependent(bool bEnable)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_BlendIndependent(bEnable);
	AdjustEnd(ibRenderCommand_BlendIndependent);
}

void ibRenderCommandList::BlendSetDesc(u32 renderTarget,
		            bool bEnable /*= false*/,
					ibBlendFactor src /*= kBlend_One*/,
					ibBlendFactor dst /*= kBlend_Zero*/,
					ibBlendOp op /*= kBlendOp_Add*/,
					ibBlendFactor srcAlpha /*= kBlend_One*/,
					ibBlendFactor dstAlpha /*= kBlend_Zero*/,
					ibBlendOp opAlpha /*= kBlendOp_Add*/,
					u8 mask /*= kWriteEnable_All*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_BlendSetDesc(renderTarget, bEnable, src, dst, op, srcAlpha, dstAlpha, opAlpha, mask);
	AdjustEnd(ibRenderCommand_BlendSetDesc);
}

void ibRenderCommandList::BlendCommit()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_BlendCommit;
	AdjustEnd(ibRenderCommand_BlendCommit);
}

// Depth stencil state
void ibRenderCommandList::DepthStencilReset()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_DepthStencilReset;
	AdjustEnd(ibRenderCommand_DepthStencilReset);
}

void ibRenderCommandList::DepthEnable(bool bEnable)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_DepthEnable(bEnable);
	AdjustEnd(ibRenderCommand_DepthEnable);
}

void ibRenderCommandList::DepthFunction(ibDepthFunction func)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_DepthFunction(func);
	AdjustEnd(ibRenderCommand_DepthFunction);
}

void ibRenderCommandList::StencilEnable(bool bEnable, u8 readMask /*= 0xFF*/, u8 writeMask /*= 0xFF*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_StencilEnable(bEnable, readMask, writeMask);
	AdjustEnd(ibRenderCommand_StencilEnable);
}

void ibRenderCommandList::StencilFrontFace(ibStencilOp fail, ibStencilOp depth, ibStencilOp pass, ibDepthFunction func)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_StencilFrontFace(fail, depth, pass, func);
	AdjustEnd(ibRenderCommand_StencilFrontFace);
}

void ibRenderCommandList::StencilBackFace(ibStencilOp fail, ibStencilOp depth, ibStencilOp pass, ibDepthFunction func)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_StencilBackFace(fail, depth, pass, func);
	AdjustEnd(ibRenderCommand_StencilBackFace);
}

void ibRenderCommandList::StencilSetReference(u32 refValue)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_StencilSetReference(refValue);
	AdjustEnd(ibRenderCommand_StencilSetReference);
}

void ibRenderCommandList::DepthStencilCommit()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_DepthStencilCommit;
	AdjustEnd(ibRenderCommand_DepthStencilCommit);
}

// Rasterizer state
void ibRenderCommandList::RasterReset()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterReset;
	AdjustEnd(ibRenderCommand_RasterReset);
}

void ibRenderCommandList::RasterWireframe(bool bWireframe)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterWireframe(bWireframe);
	AdjustEnd(ibRenderCommand_RasterWireframe);
}

void ibRenderCommandList::RasterCull(ibCullMode mode, bool ccw /*= false*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterCull(mode, ccw);
	AdjustEnd(ibRenderCommand_RasterCull);
}

void ibRenderCommandList::RasterDepthBias(s32 bias, f32 clamp, f32 slopeBias)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterDepthBias(bias, clamp, slopeBias);
	AdjustEnd(ibRenderCommand_RasterDepthBias);
}

void ibRenderCommandList::RasterDepthClip(bool bClip)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterDepthClip(bClip);
	AdjustEnd(ibRenderCommand_RasterDepthClip);
}

void ibRenderCommandList::RasterScissorEnable(bool bEnable)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterScissorEnable(bEnable);
	AdjustEnd(ibRenderCommand_RasterScissorEnable);
}

void ibRenderCommandList::RasterMultisample(bool bEnable)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterMultisample(bEnable);
	AdjustEnd(ibRenderCommand_RasterMultisample);
}

void ibRenderCommandList::RasterLineAA(bool bEnable)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterLineAA(bEnable);
	AdjustEnd(ibRenderCommand_RasterLineAA);
}

void ibRenderCommandList::RasterCommit()
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_RasterCommit();
	AdjustEnd(ibRenderCommand_RasterCommit);
}

void ibRenderCommandList::SetVertexBuffer(u32 slot, ibRenderVertexBuffer* pBuffer, u32 offset /*= 0*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_SetVertexBuffer(slot, pBuffer, offset);
	AdjustEnd(ibRenderCommand_SetVertexBuffer);
}

void ibRenderCommandList::SetIndexBuffer(ibRenderIndexBuffer* pBuffer, u32 offset /*= 0*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_SetIndexBuffer(pBuffer, offset);
	AdjustEnd(ibRenderCommand_SetIndexBuffer);
}

void ibRenderCommandList::SetTopology(ibTopology topology)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_SetTopology(topology);
	AdjustEnd(ibRenderCommand_SetTopology);
}

// Vertex shader
void ibRenderCommandList::SetVertexShaderConstants(u32 slot, ibRenderConstantBuffer* pBuffer)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_VSSetConstantBuffer(slot, pBuffer);
	AdjustEnd(ibRenderCommand_VSSetConstantBuffer);
}

void ibRenderCommandList::SetVertexShaderSampler(u32 slot, ibRenderSampler* pSampler)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_VSSetSampler(slot, pSampler);
	AdjustEnd(ibRenderCommand_VSSetSampler);
}

void ibRenderCommandList::SetVertexShaderResource(u32 slot, ibRenderTexture* pTexture)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_VSSetResource(slot, pTexture);
	AdjustEnd(ibRenderCommand_VSSetResource);
}

void ibRenderCommandList::SetVertexShader(ibRenderShader& pShader)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_VSSetShader(pShader);
	AdjustEnd(ibRenderCommand_VSSetShader);
}

// Pixel shader
void ibRenderCommandList::SetPixelShaderConstants(u32 slot, ibRenderConstantBuffer* pBuffer)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_PSSetConstantBuffer(slot, pBuffer);
	AdjustEnd(ibRenderCommand_PSSetConstantBuffer);
}

void ibRenderCommandList::SetPixelShaderSampler(u32 slot, ibRenderSampler* pSampler)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_PSSetSampler(slot, pSampler);
	AdjustEnd(ibRenderCommand_PSSetSampler);
}

void ibRenderCommandList::SetPixelShaderResource(u32 slot, ibRenderTexture* pTexture)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_PSSetResource(slot, pTexture);
	AdjustEnd(ibRenderCommand_PSSetResource);
}

void ibRenderCommandList::SetPixelShader(ibRenderShader& pShader)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_PSSetShader(pShader);
	AdjustEnd(ibRenderCommand_PSSetShader);
}

// Rasterizer stage
//void SetViewports(u32 count, ibRenderViewport* pViewports);
//void SetScissors(u32 count, ibRenderScissor* pScissors);

// Render targets (output merger)
void ibRenderCommandList::SetRenderTargets(ibRenderTarget* pTargets, ibRenderDepthStencil* pDepthStencil, u32 numViews /*= 0*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_SetRenderTargets(pTargets, pDepthStencil, numViews);
	AdjustEnd(ibRenderCommand_SetRenderTargets);
}

// Batch commit
void ibRenderCommandList::Draw(u32 vertexCount, u32 vertexStart /*= 0*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_Draw(vertexCount, vertexStart);
	AdjustEnd(ibRenderCommand_Draw);
}

void ibRenderCommandList::DrawIndexed(u32 indexCount, u32 start, int vertexOffset /*= 0*/)
{
	ibRenderCommand* pCmd = new (m_pEnd) ibRenderCommand_DrawIndexed(indexCount, start, vertexOffset);
	AdjustEnd(ibRenderCommand_DrawIndexed);
}
