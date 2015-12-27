#ifndef IB_RENDER_DEVICE_H
#define IB_RENDER_DEVICE_H
#pragma once

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#endif

#include "ibColor.h"
//#include "ibRenderSampler.h"
#include "ibRenderBuffer.h"
#include "ibRenderShader.h"
#include "ibRenderTarget.h"
#include "ibRenderDepthStencil.h"

#include "ibRenderDefs.h"

class ibRenderIndexBuffer;
class ibRenderVertexBuffer;
class ibRenderConstantBuffer;
class ibRenderSampler;
class ibRenderTexture;

struct ibSamplerDesc {
	ibTextureFilter filter;
	ibTextureAddressMode address_u;
	ibTextureAddressMode address_v;
	ibTextureAddressMode address_w;
	ibFilterCompare compare;
	float lod_bias;
	float lod_min;
	float lod_max;
	ibColorF border;
};

class IB_RENDER_EXPORT ibRenderDevice
{
public:
	ibRenderDevice();
	~ibRenderDevice();

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Render commands (immediate)
	///////////////////////////////////////////////////////////////////////////////////////////////
	void ClearImmediate(const float color[4], float depth, u8 stencil, bool clearState = false);
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
	void VSSetConstantBuffer(u32 slot, ibRenderConstantBuffer* pBuffer);
	void VSSetSampler(u32 slot, ibRenderSampler* pSampler);
	void VSSetResource(u32 slot, ibRenderTexture* pTexture);
	void VSSetShader(ibRenderShader& pShader);

	// Pixel shader
	void PSSetConstantBuffer(u32 slot, ibRenderConstantBuffer* pBuffer);
	void PSSetSampler(u32 slot, ibRenderSampler* pSampler);
	void PSSetResource(u32 slot, ibRenderTexture* pTexture);
	void PSSetShader(ibRenderShader& pShader);

	// Rasterizer stage
	//void SetViewports(u32 count, ibRenderViewport* pViewports);
	//void SetScissors(u32 count, ibRenderScissor* pScissors);

	// Render targets (output merger)
	void SetRenderTargets(ibRenderTarget* pTargets, ibRenderDepthStencil* pDepthStencil, u32 numViews = 0);

	// Batch commit
	void Draw(u32 vertexCount, u32 vertexStart = 0);
	void DrawIndexed(u32 indexCount, u32 start, int vertexOffset = 0);

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Resource creation (unmanaged)
	///////////////////////////////////////////////////////////////////////////////////////////////
	ibRenderSampler CreateSampler(const ibSamplerDesc& desc);
	
	ibRenderShader CreateShader(ibShaderType type, const void* pBytecode, u32 size);
	void SetShader(ibRenderShader* pShader);

	ibRenderConstantBuffer CreateConstantBuffer(u32 size, void* pData = 0, ibBufferUsage = kUsage_Dynamic);
	ibRenderIndexBuffer    CreateIndexBuffer(u32 count, u32* indices = 0, ibBufferUsage = kUsage_Default);
	ibRenderIndexBuffer    CreateIndexBuffer(u32 count, u16* indices, ibBufferUsage = kUsage_Default);
	ibRenderVertexBuffer   CreateVertexBuffer(
		u32 vertSize,
		u32 vertCount,
		void* pData = 0,
		ibBufferUsage usage = kUsage_Default
	);

	void SetMsaaSampleCount(u32 samples);
	void SetMsaaQuality(u32 quality);

	ibRenderTexture CreateTexture1D(
		ibFormat format,
		u32 width,
		u32 mips,
		u32* pitches, // u32 pitches[mips]
		void** pMips = 0, // void* mipData[mips]
		bool generateMips = false,
		ibTextureUsage usage = kUsage_Default
	);

	ibRenderTexture CreateTexure2D(
		ibFormat format,
		u32 width,
		u32 height,
		u32 mips,
		u32* pitches, // u32 pitches[mips]
		void** pMips = 0, // void* mipData[mips]
		bool generateMips = false,
		ibTextureUsage usage = kUsage_Default
	);

	ibRenderTarget CreateRenderTarget(
		ibFormat format, 
		u32 width,
		u32 height,
		u32 mips = 1,
		bool generateMips = false,
		bool cpuWrite = false,
		ibTextureUsage usage = kUsage_Default
	);

	ibRenderDepthStencil CreateDepthStencil(
		ibFormat format,
		ibRenderTarget& renderTarget,
		bool cpuWrite = false,
		ibTextureUsage usage = kUsage_Default
	);

private:
	void PlatformInit();

	void SetFlags(u32 flags);
	void ClearFlags(u32 flags);
	u32 CheckFlags(u32 flags);
	u32 CheckAllFlags(u32 flags);

#ifdef IB_RENDER_DX11
	bool CreateDeviceAndContext();
	bool CreateSwapChain();
	void InitMSAAQualities();

	ComPtr<IDXGIFactory1> m_pFactory;
	ComPtr<IDXGIAdapter> m_pAdapter;
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

	// States
	ComPtr<ID3D11BlendState> m_pBlendState;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	ComPtr<ID3D11RasterizerState> m_pRasterizerState;

	D3D_FEATURE_LEVEL m_featureLevel;

	// Descs
	D3D11_BLEND_DESC m_blendDesc;
	D3D11_DEPTH_STENCIL_DESC m_stencilDesc;
	u32 m_stencilReference;
	D3D11_RASTERIZER_DESC m_rasterizerDesc;

	// Msaa settings
	u32 m_msaaMaxRTSamples;
	u32 m_msaaMaxRTQuality;
	u32 m_msaaSampleSetting;
	u32 m_msaaQualitySetting;

#endif // IB_RENDER_DX11

	u32 m_flags;
};

#endif // IB_RENDER_DEVICE_H
