#include "ibRenderDevice.h"

#include "ibLog.h"
#include "ibSystem.h"

#include "DXGI.h"

#include "ibRenderConstantBuffer.h"
#include "ibRenderIndexBuffer.h"
#include "ibRenderVertexBuffer.h"
#include "ibRenderSampler.h"
#include "ibRenderShader.h"
#include "ibRenderTexture.h"

namespace
{
	const u32 kRenderDeviceFlags_BlendDirty  = 0x1000;
	const u32 kRenderDeviceFlags_DepthDirty  = 0x2000;
	const u32 kRenderDeviceFlags_RasterDirty = 0x4000;

#ifndef NDEBUG
	void LogAdapterDesc(DXGI_ADAPTER_DESC1* pDesc, u32 idx)
	{
			ibLog("[RENDER] Adapter %u:\n"
				  "[RENDER]\tDesc: %S\n"
				  "[RENDER]\tVendorId: %u\n"
				  "[RENDER]\tDeviceId: %u\n"
				  "[RENDER]\tSubSysId: %u\n"
				  "[RENDER]\tRevision: %u\n"
				  "[RENDER]\tDedicated video memory: %u\n"
				  "[RENDER]\tDedicated system memory: %u\n"
				  "[RENDER]\tShared system memory: %u\n"
				  "[RENDER]\tLUID: %X-%X\n"
				  "[RENDER]\tFlags: %u\n",
				  idx, pDesc->Description, pDesc->VendorId,
				  pDesc->DeviceId, pDesc->SubSysId, pDesc->Revision,
				  pDesc->DedicatedVideoMemory, pDesc->DedicatedSystemMemory,
				  pDesc->SharedSystemMemory, 
				  pDesc->AdapterLuid.HighPart, pDesc->AdapterLuid.LowPart,
				  pDesc->Flags);
	}

	void LogOutputDesc(DXGI_OUTPUT_DESC* pDesc, int idx)
	{
		ibLog("[RENDER]\t\tOutput %u:\n"
			"[RENDER]\t\t\tName: %S\n"
			"[RENDER]\t\t\tCoords:\n"
			"[RENDER]\t\t\t\t{ top: %d, left: %d, bottom: %d, right: %d }\n"
			"[RENDER]\t\t\tAttached to desktop: %s\n"
			"[RENDER]\t\t\tRotation: %u\n",
			idx, pDesc->DeviceName,  
			pDesc->DesktopCoordinates.top, pDesc->DesktopCoordinates.left,
			pDesc->DesktopCoordinates.bottom, pDesc->DesktopCoordinates.right,
			pDesc->AttachedToDesktop ? "TRUE" : "FALSE", pDesc->Rotation);
	}

	void EnumAdaptersAndOutputs()
	{
		ComPtr<IDXGIFactory1> pFactory;
		CreateDXGIFactory1(__uuidof(IDXGIFactory1), pFactory);
		ibAssert(*pFactory);
		u32 i = 0;
		ComPtr<IDXGIAdapter1> pAdapter;
		while (pFactory->EnumAdapters1(i, pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 Desc;
			pAdapter->GetDesc1(&Desc);
			LogAdapterDesc(&Desc, i);
		
			u32 n = 0;
			ComPtr<IDXGIOutput> pOutput;
			while (pAdapter->EnumOutputs(n, pOutput) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_OUTPUT_DESC outputDesc;
				pOutput->GetDesc(&outputDesc);
				LogOutputDesc(&outputDesc, n);

				++n;
				pOutput->Release();
			}

			++i;
			pAdapter->Release();
			ibLog("[RENDER] --------------------------------------------------------------------\n");
		}
	}
#else
	void EnumAdaptersAndOuputs() {}
#endif // NDEBUG

	template <class T>
	ComPtr<ID3D11DeviceChild> CreateShader_internal(
		ComPtr<ID3D11Device> device,
		const void* pBytecode,
		u32 codeSize,
		HRESULT (__stdcall ID3D11Device::*Creationism)(const void*, SIZE_T, ID3D11ClassLinkage*, T**))
	{
		ComPtr<ID3D11DeviceChild> pBase;
		ComPtr<T> outT;
		HRESULT hr = ((*device)->*Creationism)(pBytecode, codeSize, NULL, outT);
		if (SUCCEEDED(hr))
			outT->QueryInterface(__uuidof(ID3D11DeviceChild), pBase);
		return pBase;
	}

	struct CreateShader_internal_binding
	{
		CreateShader_internal_binding(ComPtr<ID3D11Device> d, const void* pb, u32 size):m_pd(d), m_pb(pb), m_size(size) {}

		template <class T>
		ComPtr<ID3D11DeviceChild> operator()(HRESULT (__stdcall ID3D11Device::*Create)(const void*, SIZE_T, ID3D11ClassLinkage*, T**)) {
			return CreateShader_internal(m_pd, m_pb, m_size, Create);
		}

		ComPtr<ID3D11Device> m_pd;
		const void* m_pb;
		u32 m_size;
	};

	const u32 kSampleCountEnd = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT + 1;

	struct ibFormatMap {
		ibFormat format;
		DXGI_FORMAT dxgiFormat;
		DXGI_FORMAT dxgiTypeless;
		u32 msaaQualities[kSampleCountEnd];
	} MappedFormats[kFormat_Count] = {
		  // Format,       DXGI Format,                      DXGI Typeless format
		{ kFormat_RGBA,    DXGI_FORMAT_R8G8B8A8_UNORM,       DXGI_FORMAT_R8G8B8A8_TYPELESS },
		{ kFormat_R8,      DXGI_FORMAT_R8_UNORM,             DXGI_FORMAT_R8_TYPELESS       },
		{ kFormat_DXT1,    DXGI_FORMAT_BC1_UNORM,            DXGI_FORMAT_BC1_TYPELESS      },
		{ kFormat_DXT2,    DXGI_FORMAT_BC2_UNORM,            DXGI_FORMAT_BC2_TYPELESS      },
		{ kFormat_DXT3,    DXGI_FORMAT_BC3_UNORM,            DXGI_FORMAT_BC3_TYPELESS      },
		{ kFormat_DXT4,    DXGI_FORMAT_BC4_UNORM,            DXGI_FORMAT_BC4_TYPELESS      },
		{ kFormat_DXT5,    DXGI_FORMAT_BC5_UNORM,            DXGI_FORMAT_BC5_TYPELESS      },
		{ kFormat_F16,     DXGI_FORMAT_R16_FLOAT,            DXGI_FORMAT_R16_TYPELESS      },
		{ kFormat_F32,     DXGI_FORMAT_R32_FLOAT,            DXGI_FORMAT_R32_TYPELESS      },
		{ kFormat_Depth,   DXGI_FORMAT_D24_UNORM_S8_UINT,    DXGI_FORMAT_R24G8_TYPELESS    },
		{ kFormat_Depth32, DXGI_FORMAT_D32_FLOAT_S8X24_UINT, DXGI_FORMAT_R32G8X24_TYPELESS },
	};

	DXGI_FORMAT FormatToDXGIFormat(ibFormat format)
	{
		return MappedFormats[format].dxgiFormat;
	}

	DXGI_FORMAT FormatToDXGITypeless(ibFormat format)
	{
		return MappedFormats[format].dxgiTypeless;
	}
}

void ibRenderDevice::PlatformInit()
{
	if (!ibSystem::GetMainWindow())
		return;
	
	EnumAdaptersAndOutputs();

	if (CreateDeviceAndContext())
	{
		ComPtr<IDXGIDevice1> pDevice;
		HRESULT hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice1), pDevice);
		if (*pDevice)
			hr = pDevice->GetParent(__uuidof(IDXGIAdapter), m_pAdapter);
		if (*m_pAdapter && !*m_pFactory)
			hr = m_pAdapter->GetParent(__uuidof(IDXGIFactory1), m_pFactory);
		ibAssert(*m_pAdapter && *m_pFactory);

		InitMSAAQualities();
		CreateSwapChain();
	}
}

bool ibRenderDevice::CreateDeviceAndContext()
{
	D3D11_CREATE_DEVICE_FLAG deviceFlags = D3D11_CREATE_DEVICE_FLAG(0);
#ifndef NDEBUG
	//deviceFlags = (D3D11_CREATE_DEVICE_FLAG)(D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_DEBUGGABLE);
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif
#ifdef IB_RENDER_PREVENT_THREADING
	deviceFlags = (D3D11_CREATE_DEVICE_FLAG)(deviceFlags | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS);
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	u32 nLevels = _countof(featureLevels);

	HRESULT hr = D3D11CreateDevice(
		*m_pAdapter,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		featureLevels,
		nLevels,
		D3D11_SDK_VERSION,
		m_pDevice,
		&m_featureLevel,
		m_pContext);
	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags,
			                   featureLevels + 1, nLevels - 1, D3D11_SDK_VERSION,
							   m_pDevice, &m_featureLevel, m_pContext);
	}
	if (FAILED(hr))
	{
		ibLog("[RENDER] Unable to create D3D11 device and context: %u\n", hr);
	}

	return SUCCEEDED(hr);
}

bool ibRenderDevice::CreateSwapChain()
{
	ibAssert(*m_pFactory);
	ComPtr<IDXGIFactory2> pFactory2;
	HRESULT hr = m_pFactory->QueryInterface(__uuidof(IDXGIFactory2), pFactory2);

	//if (*pFactory2)
	//{
	//}
	//else
	{
		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		DXGI_MODE_DESC& modeDesc = scd.BufferDesc;
		DXGI_SAMPLE_DESC& sampleDesc = scd.SampleDesc;
		modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 2;
		scd.OutputWindow = ibSystem::GetMainWindow();
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		hr = m_pFactory->CreateSwapChain(*m_pDevice, &scd, m_pSwapChain);
		if (SUCCEEDED(hr))
		{
			ComPtr<ID3D11Texture2D> pBackBuffer;
			m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), pBackBuffer);
			hr = m_pDevice->CreateRenderTargetView(*pBackBuffer, NULL, m_pRenderTargetView);
		}
	}

	return !!*m_pSwapChain;
}

void ibRenderDevice::InitMSAAQualities()
{
	for (u32 n = 0; n < kFormat_Count; ++n)
	{
		for (u32 sample = 0; sample < kSampleCountEnd; ++sample)
		{
			HRESULT hr = m_pDevice->CheckMultisampleQualityLevels(MappedFormats[n].dxgiFormat, sample, &MappedFormats[n].msaaQualities[sample]);
			ibAssert(SUCCEEDED(hr));
		}
	}

	// Find the max render target samples and quality
	for (u32 sample = 0; sample < kSampleCountEnd; ++sample)
	{
		// Use the lowest quality for this sample setting.  If any are 0 we don't support
		// this MSAA format for render targets.
		u32 quality = ibMin(ibMin(MappedFormats[kFormat_RGBA].msaaQualities[sample],
			                      MappedFormats[kFormat_Depth].msaaQualities[sample]),
								  MappedFormats[kFormat_Depth32].msaaQualities[sample]);
		if (quality)
		{
			m_msaaMaxRTSamples = sample;
			if (m_msaaMaxRTQuality && quality < m_msaaMaxRTQuality)
				m_msaaMaxRTQuality = quality;
		}
	}

	// Set default MSAA to 1/0
	m_msaaSampleSetting = 1;
	m_msaaQualitySetting = 0;
}

void ibRenderDevice::ClearImmediate(const float color[4], float depth, u8 stencil, bool clearState)
{
	ibUnused(depth);
	ibUnused(stencil);

	if (*m_pRenderTargetView)
		m_pContext->ClearRenderTargetView(*m_pRenderTargetView, color);
	//if (m_pDepthStencil)
	//	m_pContext->ClearDepthStencilView(*m_pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
	if (clearState)
		m_pContext->ClearState();
}

void ibRenderDevice::Present()
{
	if (*m_pSwapChain)
		m_pSwapChain->Present(0, 0);
}

void ibRenderDevice::BlendReset()
{
	m_blendDesc.AlphaToCoverageEnable = FALSE;
	m_blendDesc.IndependentBlendEnable = FALSE;

	for (u32 n = 0; n < 8; ++n)
		BlendSetDesc(n);

	// Flag set in BlendSetDesc
	//SetFlags(kRenderDeviceFlags_BlendDirty);
}

void ibRenderDevice::BlendAlphaToCoverage(bool bEnable)
{
	m_blendDesc.AlphaToCoverageEnable = bEnable;
	SetFlags(kRenderDeviceFlags_BlendDirty);
}

void ibRenderDevice::BlendIndependent(bool bEnable)
{
	m_blendDesc.IndependentBlendEnable = bEnable;
	SetFlags(kRenderDeviceFlags_BlendDirty);
}

void ibRenderDevice::BlendSetDesc(u32 renderTarget,
								  bool bEnable,
								  ibBlendFactor src,
								  ibBlendFactor dst,
								  ibBlendOp op,
								  ibBlendFactor srcAlpha,
								  ibBlendFactor dstAlpha,
								  ibBlendOp opAlpha,
								  u8 mask)
{
	ibAssert(renderTarget < IB_MAX_RENDER_TARGET_DESCS);
	ibAssert(src < kBlend_Count);
	ibAssert(dst < kBlend_Count);
	ibAssert(op < kBlendOp_Count);
	ibAssert(srcAlpha < kBlend_Count);
	ibAssert(dstAlpha < kBlend_Count);
	ibAssert(opAlpha < kBlendOp_Count);

	D3D11_RENDER_TARGET_BLEND_DESC& rtbd = m_blendDesc.RenderTarget[renderTarget];
	rtbd.BlendEnable = bEnable;
	rtbd.SrcBlend = D3D11_BLEND(src + 1);
	rtbd.DestBlend = D3D11_BLEND(dst + 1);
	rtbd.BlendOp = D3D11_BLEND_OP(op + 1);
	rtbd.SrcBlendAlpha = D3D11_BLEND(srcAlpha + 1);
	rtbd.DestBlendAlpha = D3D11_BLEND(dstAlpha + 1);
	rtbd.BlendOpAlpha = D3D11_BLEND_OP(opAlpha + 1);
	rtbd.RenderTargetWriteMask = mask;

	SetFlags(kRenderDeviceFlags_BlendDirty);
}

void ibRenderDevice::BlendCommit()
{
	if (CheckFlags(kRenderDeviceFlags_BlendDirty))
	{
		ComPtr<ID3D11BlendState> pState;
		HRESULT hr = m_pDevice->CreateBlendState(&m_blendDesc, pState);
		ibAssert(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			m_pBlendState = pState;
			m_pContext->OMSetBlendState(*m_pBlendState, NULL, 0xFFFFFFFF);
			ClearFlags(kRenderDeviceFlags_BlendDirty); // Do not clear flags if we failed
		}
	}
}

void ibRenderDevice::DepthStencilReset()
{
	m_stencilDesc.DepthEnable = TRUE;
	m_stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER; // This is different from the DX11 default because we use 1.0 for near
	m_stencilDesc.StencilEnable = FALSE;
	m_stencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_stencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	m_stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::DepthEnable(bool bEnable)
{
	m_stencilDesc.DepthEnable = bEnable;
	m_stencilDesc.DepthWriteMask = bEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::DepthFunction(ibDepthFunction func)
{
	ibAssert(func < kCompare_Count);
	m_stencilDesc.DepthFunc = D3D11_COMPARISON_FUNC(func + 1);
	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::StencilEnable(bool bEnable, u8 readMask /*= 0xFF*/, u8 writeMask /*= 0xFF*/)
{
	m_stencilDesc.StencilEnable = bEnable;
	m_stencilDesc.StencilReadMask = readMask;
	m_stencilDesc.StencilWriteMask = writeMask;

	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::StencilFrontFace(ibStencilOp fail, ibStencilOp depth, ibStencilOp pass, ibDepthFunction func)
{
	ibAssert(fail < kStencil_Count);
	ibAssert(depth < kStencil_Count);
	ibAssert(pass < kStencil_Count);
	ibAssert(func < kCompare_Count);

	m_stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP(fail + 1);
	m_stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP(depth + 1);
	m_stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP(pass + 1);
	m_stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC(func + 1);

	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::StencilBackFace(ibStencilOp fail, ibStencilOp depth, ibStencilOp pass, ibDepthFunction func)
{
	ibAssert(fail < kStencil_Count);
	ibAssert(depth < kStencil_Count);
	ibAssert(pass < kStencil_Count);
	ibAssert(func < kCompare_Count);

	m_stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP(fail + 1);
	m_stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP(depth + 1);
	m_stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP(pass + 1);
	m_stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC(func + 1);

	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::StencilSetReference(u32 refValue)
{
	m_stencilReference = refValue;
	SetFlags(kRenderDeviceFlags_DepthDirty);
}

void ibRenderDevice::DepthStencilCommit()
{
	if (CheckFlags(kRenderDeviceFlags_DepthDirty))
	{
		ComPtr<ID3D11DepthStencilState> pState;
		HRESULT hr = m_pDevice->CreateDepthStencilState(&m_stencilDesc, pState);
		ibAssert(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			m_pDepthStencilState = pState;
			m_pContext->OMSetDepthStencilState(*m_pDepthStencilState, m_stencilReference);
			ClearFlags(kRenderDeviceFlags_DepthDirty); // Do not clear flags if we failed
		}
	}
}

void ibRenderDevice::RasterReset()
{
	m_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
	m_rasterizerDesc.FrontCounterClockwise = FALSE;
	m_rasterizerDesc.DepthBias = 0;
	m_rasterizerDesc.DepthBiasClamp = 0.f;
	m_rasterizerDesc.SlopeScaledDepthBias = 0.f;
	m_rasterizerDesc.DepthClipEnable = TRUE;
	m_rasterizerDesc.ScissorEnable = FALSE;
	m_rasterizerDesc.MultisampleEnable = FALSE;
	m_rasterizerDesc.AntialiasedLineEnable = FALSE;

	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterWireframe(bool bWireframe)
{
	m_rasterizerDesc.FillMode = bWireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterCull(ibCullMode mode, bool ccw /*= false*/)
{
	m_rasterizerDesc.CullMode = D3D11_CULL_MODE(mode + 1);
	m_rasterizerDesc.FrontCounterClockwise = ccw;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterDepthBias(s32 bias, f32 clamp, f32 slopeBias)
{
	m_rasterizerDesc.DepthBias = bias;
	m_rasterizerDesc.DepthBiasClamp = clamp;
	m_rasterizerDesc.SlopeScaledDepthBias = slopeBias;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterDepthClip(bool bClip)
{
	m_rasterizerDesc.DepthClipEnable = bClip;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterScissorEnable(bool bEnable)
{
	m_rasterizerDesc.ScissorEnable = bEnable;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterMultisample(bool bEnable)
{
	m_rasterizerDesc.MultisampleEnable = bEnable;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterLineAA(bool bEnable)
{
	m_rasterizerDesc.AntialiasedLineEnable = bEnable;
	SetFlags(kRenderDeviceFlags_RasterDirty);
}

void ibRenderDevice::RasterCommit()
{
	if (CheckFlags(kRenderDeviceFlags_RasterDirty))
	{
		ComPtr<ID3D11RasterizerState> pState;
		HRESULT hr = m_pDevice->CreateRasterizerState(&m_rasterizerDesc, pState);
		ibAssert(SUCCEEDED(hr));
		if (SUCCEEDED(hr))
		{
			m_pRasterizerState = pState;
			m_pContext->RSSetState(*m_pRasterizerState);
			ClearFlags(kRenderDeviceFlags_RasterDirty); // Do not clea flags if we failed
		}
	}
}

ibRenderSampler ibRenderDevice::CreateSampler(const ibSamplerDesc& desc)
{
	ibAssert(desc.filter < kFilter_Count);
	ibAssert(desc.address_u < kTexture_Count);
	ibAssert(desc.address_v < kTexture_Count);
	ibAssert(desc.address_w < kTexture_Count);
	ibAssert(desc.compare < kCompare_Count);

	D3D11_FILTER filterMap[] = {
		D3D11_FILTER_MIN_MAG_MIP_POINT, // kFilter_Point
		D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, // kFilter_Bilinear
		D3D11_FILTER_MIN_MAG_MIP_LINEAR, // kFilter_Trilinear
		D3D11_FILTER_ANISOTROPIC, // kFilter_Anisotropic
	};

	D3D11_SAMPLER_DESC dxDesc;
	dxDesc.Filter = filterMap[desc.filter];
	dxDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE(desc.address_u + 1);
	dxDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE(desc.address_v + 1);
	dxDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE(desc.address_w + 1);
	dxDesc.MipLODBias = desc.lod_bias;
	dxDesc.MaxAnisotropy = 16;
	dxDesc.ComparisonFunc = D3D11_COMPARISON_FUNC(desc.compare + 1);
	dxDesc.MinLOD = desc.lod_min;
	dxDesc.MaxLOD = desc.lod_max;
	for (u32 n = 0; n < 4; ++n)
		dxDesc.BorderColor[n] = desc.border[n];

	ComPtr<ID3D11SamplerState> pState;
	HRESULT hr = m_pDevice->CreateSamplerState(&dxDesc, pState);
	ibAssert(SUCCEEDED(hr));

	return pState;
}

ibRenderShader ibRenderDevice::CreateShader(ibShaderType type, const void* pBytecode, u32 size)
{
	ComPtr<ID3D11DeviceChild> pBase;
	CreateShader_internal_binding CreateFunctor(m_pDevice, pBytecode, size);

	switch (type)
	{
	case kShader_Compute:
		pBase = CreateFunctor(&ID3D11Device::CreateComputeShader);
		break;

	case kShader_Domain:
		pBase = CreateFunctor(&ID3D11Device::CreateDomainShader);
		break;

	case kShader_Geometry:
		pBase = CreateFunctor(&ID3D11Device::CreateGeometryShader);
		break;

	case kShader_Hull:
		pBase = CreateFunctor(&ID3D11Device::CreateHullShader);
		break;

	case kShader_Pixel:
		pBase = CreateFunctor(&ID3D11Device::CreatePixelShader);
		break;

	case kShader_Vertex:
		pBase = CreateFunctor(&ID3D11Device::CreateVertexShader);
		break;

	default:
		ibError("Unsupported shader type requested");
	}

	ibRenderShader ret(type, pBase);
	return ret;
}

ibRenderConstantBuffer ibRenderDevice::CreateConstantBuffer(u32 size, void* pData /*= 0*/, ibBufferUsage usage /*= kUsage_Dynamic*/)
{
	ibAssertMsg(size != 0, "Cannot create 0 size buffer");
	ibAssertMsg(usage != kUsage_Immutable || pData != 0, "Cannot create immutable buffer without data");
	ibAssert(usage < kUsage_Count);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE(usage);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	if (usage == kUsage_Dynamic || usage == kUsage_Staging)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&bufferDesc, &initData, pBuffer);
	ibAssert(SUCCEEDED(hr));

	return ibRenderConstantBuffer(pBuffer, usage);
}

ibRenderIndexBuffer ibRenderDevice::CreateIndexBuffer(u32 count, u32* pIndices /*= 0*/, ibBufferUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(count != 0, "Cannot create index buffer with 0 indices");
	ibAssertMsg(usage != kUsage_Immutable || pIndices != 0, "Cannot create immutable buffer without data");
	ibAssert(usage < kUsage_Count);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(u32) * count;
	bufferDesc.Usage = D3D11_USAGE(usage);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	if (usage == kUsage_Dynamic || usage == kUsage_Staging)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pIndices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&bufferDesc, &initData, pBuffer);
	ibAssert(SUCCEEDED(hr));

	return ibRenderIndexBuffer(pBuffer, usage, kIndex_32);

}

ibRenderIndexBuffer ibRenderDevice::CreateIndexBuffer(u32 count, u16* pIndices, ibBufferUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(count != 0, "Cannot create index buffer with 0 indices");
	ibAssertMsg(usage != kUsage_Immutable || pIndices != 0, "Cannot create immutable buffer without data");
	ibAssert(usage < kUsage_Count);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(u16) * count;
	bufferDesc.Usage = D3D11_USAGE(usage);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	if (usage == kUsage_Dynamic || usage == kUsage_Staging)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pIndices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&bufferDesc, &initData, pBuffer);
	ibAssert(SUCCEEDED(hr));

	return ibRenderIndexBuffer(pBuffer, usage, kIndex_16);
}

ibRenderVertexBuffer ibRenderDevice::CreateVertexBuffer(u32 vertSize, u32 vertCount, void* pData /*= 0*/, ibBufferUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(vertSize, "Cannot create vertex buffer with 0 size vertices");
	ibAssertMsg(vertCount, "Cannot create 0 size vertex buffer");
	ibAssertMsg(usage != kUsage_Immutable || pData != 0, "Cannot create immutable buffer without data");
	ibAssert(usage < kUsage_Count);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = vertSize * vertCount;
	bufferDesc.Usage = D3D11_USAGE(usage);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	if (usage == kUsage_Dynamic || usage == kUsage_Staging)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pBuffer;
	HRESULT hr = m_pDevice->CreateBuffer(&bufferDesc, &initData, pBuffer);
	ibAssert(SUCCEEDED(hr));

	return ibRenderVertexBuffer(pBuffer, vertSize, usage);
}

ibRenderTexture ibRenderDevice::CreateTexture1D(
	ibFormat format,
	u32 width,
	u32 mips,
	u32* pitches,
	void** pMips /*= 0*/,
	bool generateMips /*= false*/,
	ibTextureUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(width, "Cannot create a 0 width texture");
	ibAssertMsg(pitches || (mips < 2 && !pMips), "Pitches cannot be null when initializing texture");
	ibAssertMsg(usage != kUsage_Immutable || pMips != 0, "Cannot create immutable texture without data");
	ibAssertMsg(mips != 1 || pMips == 0, "Cannot initialize multisampled texture with data");

	D3D11_TEXTURE1D_DESC desc;
	desc.Width = width;
	desc.MipLevels = mips;
	desc.ArraySize = 1;
	desc.Format = FormatToDXGITypeless(format);
	desc.Usage = D3D11_USAGE(usage);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = (generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);
	// If pMips is 0 we don't know the initialization path, so flag for CPU
	if ((usage == kUsage_Dynamic || usage == kUsage_Staging) || pMips == 0)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// if mip levels are automatically allocated and not-initialized, we can't know
	// if the resource(s) are CPU or GPU initialized; we already have CPU access
	// so force to render target for GPU write
	if (mips == 0 && pMips == 0)
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	D3D11_SUBRESOURCE_DATA* pSRD = 0;
	if (mips && pMips)
	{
		pSRD = new (g_engineHeap, "CreateTexture1D SRD") D3D11_SUBRESOURCE_DATA[mips];
		for (u32 n = 0; n < mips; ++n)
		{
			pSRD[n].pSysMem = pMips[n];
			pSRD[n].SysMemPitch = pitches[n];
			pSRD[n].SysMemSlicePitch = 0;
		}
	}

	ComPtr<ID3D11Resource> pRes;
	ComPtr<ID3D11Texture1D> pTexture;
	HRESULT hr = m_pDevice->CreateTexture1D(&desc, pSRD, pTexture);
	ibAssert(SUCCEEDED(hr));

	if (SUCCEEDED(hr))
	{
		hr = pTexture->QueryInterface(__uuidof(ID3D11Resource), pRes);
		ibAssert(SUCCEEDED(hr));

		if (!mips)
		{
			pTexture->GetDesc(&desc);
			// TODO, XXX, FIXME: Get mips from new desc
		}
	}

	delete [] pSRD;
	return ibRenderTexture(kTextureDim_1D, width, 1u, format, mips, pRes);
}

ibRenderTexture ibRenderDevice::CreateTexure2D(
	ibFormat format,
	u32 width,
	u32 height,
	u32 mips,
	u32* pitches, // u32 pitches[mips]
	void** pMips /*= 0*/, // void* mipData[mips]
	bool generateMips /*= false*/,
	ibTextureUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(width && height, "Cannot create a texture with null dimension");
	ibAssertMsg(pitches || (mips < 2 && !pMips), "Pitches cannot be null when initializing texture");
	ibAssertMsg(usage != kUsage_Immutable || pMips != 0, "Cannot create immutable texture without data");
	ibAssertMsg(mips != 1 || pMips == 0, "Cannot initialize multisampled texture with data");

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = mips;
	desc.ArraySize = 1;
	desc.Format = FormatToDXGITypeless(format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE(usage);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = (generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);
	// If pMips is 0 we don't know the initialization path, so flag for CPU
	if ((usage == kUsage_Dynamic || usage == kUsage_Staging) || pMips == 0)
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// if mip levels are automatically allocated and not-initialized, we can't know
	// if the resource(s) are CPU or GPU initialized; we already have CPU access
	// so force to render target for GPU write
	if (mips == 0 && pMips == 0)
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	// Setup MSAA for the texture
	if (mips == 1 && m_rasterizerDesc.MultisampleEnable)
	{
		// Check validity on the current setting.  Max quality is 1 less than the stored value
		// stored in msaaQualities[samples].  Do not attempt to "fix up" the quality setting
		if (m_msaaQualitySetting < MappedFormats[format].msaaQualities[m_msaaSampleSetting])
		{
			desc.SampleDesc.Count = m_msaaSampleSetting;
			desc.SampleDesc.Quality = m_msaaQualitySetting;
		}
	}

	D3D11_SUBRESOURCE_DATA* pSRD = 0;
	if (mips && pMips)
	{
		pSRD = new (g_engineHeap, "CreateTexture2D SRD") D3D11_SUBRESOURCE_DATA[mips];
		for (u32 n = 0; n < mips; ++n)
		{
			pSRD[n].pSysMem = pMips[n];
			pSRD[n].SysMemPitch = pitches[n];
			pSRD[n].SysMemSlicePitch = 0;
		}
	}

	ComPtr<ID3D11Resource> pRes;
	ComPtr<ID3D11Texture2D> pTexture;
	HRESULT hr = m_pDevice->CreateTexture2D(&desc, pSRD, pTexture);
	ibAssert(SUCCEEDED(hr));

	if (SUCCEEDED(hr))
	{
		hr = pTexture->QueryInterface(__uuidof(ID3D11Resource), pRes);
		ibAssert(SUCCEEDED(hr));

		if (!mips)
		{
			pTexture->GetDesc(&desc);
			// TODO, XXX, FIXME: Get mips from new desc
		}
	}

	delete [] pSRD;
	return ibRenderTexture(kTextureDim_2D, width, height, format, mips, pRes);
}

ibRenderTarget ibRenderDevice::CreateRenderTarget(
	ibFormat format, 
	u32 width,
	u32 height,
	u32 mips /*= 1*/,
	bool generateMips /*= false*/,
	bool cpuWrite /*= false*/,
	ibTextureUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(width && height, "Cannot create a render target with null dimension");
	ibAssertMsg(usage != kUsage_Immutable, "Cannot create immutable render target");

	///////////////////////////////////////////////
	// Create the backing texture
	///////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = mips;
	desc.ArraySize = 1;
	desc.Format = FormatToDXGIFormat(format); // Do not initialize RT texture as typeless
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE(usage);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = cpuWrite ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = (generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0);

	// Setup MSAA for the texture
	if (mips == 1 && m_rasterizerDesc.MultisampleEnable)
	{
		// Check validity on the current setting.  Max quality is 1 less than the stored value
		// stored in msaaQualities[samples] for the RT format AND the DS format; if any are
		// zero ignore MSAA settings (cannot have MSAA on RT if DS format doesn't support it).
		// Do not try to "fix up" invalid settings
		u32 maxQuality = ibMin(ibMin(MappedFormats[kFormat_RGBA].msaaQualities[m_msaaSampleSetting],
			                         MappedFormats[kFormat_Depth].msaaQualities[m_msaaSampleSetting]),
									 MappedFormats[kFormat_Depth32].msaaQualities[m_msaaSampleSetting]);

		if (m_msaaQualitySetting < maxQuality)
		{
			desc.SampleDesc.Count = m_msaaSampleSetting;
			desc.SampleDesc.Quality = m_msaaQualitySetting;
		}
	}

	ComPtr<ID3D11RenderTargetView> pRenderTarget;
	ComPtr<ID3D11Texture2D> pTexture;
	ComPtr<ID3D11Resource> pRes;
	HRESULT hr = m_pDevice->CreateTexture2D(&desc, NULL, pTexture);
	ibAssert(SUCCEEDED(hr));

	hr = pTexture->QueryInterface(__uuidof(ID3D11Resource), pRes);
	ibAssert(SUCCEEDED(hr));

	///////////////////////////////////////////////
	// Create the render target view
	///////////////////////////////////////////////
	if (SUCCEEDED(hr))
	{
		if (!mips)
		{
			pTexture->GetDesc(&desc);
			// TODO, XXX, FIXME: Get mips from new desc
		}

		D3D11_RENDER_TARGET_VIEW_DESC descRtv;
		descRtv.Format = desc.Format;
		descRtv.ViewDimension = (mips == 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
		descRtv.Texture2D.MipSlice = 0;

		hr = m_pDevice->CreateRenderTargetView(*pRes, &descRtv, pRenderTarget);
		ibAssert(SUCCEEDED(hr));
	}

	return ibRenderTarget(ibRenderTexture(kTextureDim_2D, width, height, format, mips, pRes), pRenderTarget, desc.SampleDesc.Count, desc.SampleDesc.Quality);
}

ibRenderDepthStencil ibRenderDevice::CreateDepthStencil(
	ibFormat format, 
	ibRenderTarget& renderTarget,
	bool cpuWrite /*= false*/,
	ibTextureUsage usage /*= kUsage_Default*/)
{
	ibAssertMsg(usage != kUsage_Immutable, "Cannot create immutable depth stencil");
	ibAssertMsg(format == kFormat_Depth || format == kFormat_Depth32, "Invalid depth stencil format");

	u32 mips = renderTarget.GetMips();

	///////////////////////////////////////////////
	// Create the backing texture
	///////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = renderTarget.GetWidth();
	desc.Height = renderTarget.GetHeight();
	desc.MipLevels = mips;
	desc.ArraySize = 1;
	desc.Format = FormatToDXGIFormat(format); // Do not initialize DS texture as typeless
	desc.Usage = D3D11_USAGE(usage);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = cpuWrite ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = 0;

	// MSAA settings have to be vetted by the render target already
	renderTarget.GetMSAASettings(&desc.SampleDesc.Count, &desc.SampleDesc.Quality);

	ComPtr<ID3D11DepthStencilView> pDepthStencil;
	ComPtr<ID3D11Texture2D> pTexture;
	ComPtr<ID3D11Resource> pRes;
	HRESULT hr = m_pDevice->CreateTexture2D(&desc, NULL, pTexture);
	ibAssert(SUCCEEDED(hr));

	hr = pTexture->QueryInterface(__uuidof(ID3D11Resource), pRes);
	ibAssert(SUCCEEDED(hr));

	///////////////////////////////////////////////
	// Create the depth stencil view
	///////////////////////////////////////////////
	if (SUCCEEDED(hr))
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descDs;
		descDs.Format = desc.Format;
		descDs.ViewDimension = (mips == 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		descDs.Flags = 0;
		descDs.Texture2D.MipSlice = 0;

		hr = m_pDevice->CreateDepthStencilView(*pRes, &descDs, pDepthStencil);
		ibAssertMsg(SUCCEEDED(hr), "Failed to create depth stencil view");
	}

	return ibRenderDepthStencil(ibRenderTexture(kTextureDim_2D, desc.Width, desc.Height, format, mips, pRes), pDepthStencil, desc.SampleDesc.Count, desc.SampleDesc.Quality);
}

// Input assembler -- The input layout is bound by the vertex shader
void ibRenderDevice::SetVertexBuffer(u32 slot, ibRenderVertexBuffer* pBuffer, u32 offset /*= 0*/)
{
	u32 stride = pBuffer->GetStride();
	u32 offsetBytes = offset * stride;
	m_pContext->IASetVertexBuffers(slot, 1, pBuffer->m_pBuffer, &stride, &offsetBytes);
}

void ibRenderDevice::SetIndexBuffer(ibRenderIndexBuffer* pBuffer, u32 offset /*= 0*/)
{
	ibIndexFormat fmt = pBuffer->GetFormat();
	m_pContext->IASetIndexBuffer(*pBuffer->m_pBuffer,
		                         ((fmt == kIndex_16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT),
								 offset * sizeof(u16) * (fmt + 1));
}

void ibRenderDevice::SetTopology(ibTopology topology)
{
	D3D11_PRIMITIVE_TOPOLOGY dxTopos[kTopology_Count] = {
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	};

	m_pContext->IASetPrimitiveTopology(dxTopos[topology]);
}

// Vertex shader
void ibRenderDevice::VSSetConstantBuffer(u32 slot, ibRenderConstantBuffer* pBuffer)
{
	ibAssert(pBuffer);
	m_pContext->VSSetConstantBuffers(slot, 1, pBuffer->m_pBuffer);
}

void ibRenderDevice::VSSetSampler(u32 slot, ibRenderSampler* pSampler)
{
	ibAssert(pSampler);
	m_pContext->VSSetSamplers(slot, 1, pSampler->m_pSampler);
}

void ibRenderDevice::VSSetResource(u32 slot, ibRenderTexture* pTexture)
{
	ibAssertMsg(pTexture && pTexture->isValid(), "Binding invalid texture");

	if (!*pTexture->m_pSRV && pTexture->isValid())
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = FormatToDXGIFormat(pTexture->GetFormat());
		desc.ViewDimension = (pTexture->GetType() == kTextureDim_1D) ? D3D11_SRV_DIMENSION_TEXTURE1D :
			(pTexture->GetMips() == 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = pTexture->GetMips() - 1;
		desc.Texture2D.MipLevels = pTexture->GetMips();

		HRESULT hr = m_pDevice->CreateShaderResourceView(*pTexture->m_pTexture, &desc, pTexture->m_pSRV);
		ibAssertMsg(SUCCEEDED(hr), "Unable to create shader resource view for texture");
	}

	m_pContext->VSSetShaderResources(slot, 1, pTexture->m_pSRV);
}

void ibRenderDevice::VSSetShader(ibRenderShader& pShader)
{
	ComPtr<ID3D11VertexShader> pPS;
	HRESULT hr = pShader.m_pShader->QueryInterface(__uuidof(ID3D11VertexShader), pPS);
	ibAssert(SUCCEEDED(hr));

#pragma STATIC_ERROR("Get/Load vertex formet")

	m_pContext->VSSetShader(*pPS, NULL, 0);
}

// Pixel shader
void ibRenderDevice::PSSetConstantBuffer(u32 slot, ibRenderConstantBuffer* pBuffer)
{
	ibAssert(pBuffer);
	m_pContext->PSSetConstantBuffers(slot, 1, pBuffer->m_pBuffer);
}

void ibRenderDevice::PSSetSampler(u32 slot, ibRenderSampler* pSampler)
{
	ibAssert(pSampler);
	m_pContext->PSSetSamplers(slot, 1, pSampler->m_pSampler);
}

void ibRenderDevice::PSSetResource(u32 slot, ibRenderTexture* pTexture)
{
	ibAssertMsg(pTexture && pTexture->isValid(), "Binding invalid texture");

	if (!*pTexture->m_pSRV && pTexture->isValid())
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format = FormatToDXGIFormat(pTexture->GetFormat());
		desc.ViewDimension = (pTexture->GetType() == kTextureDim_1D) ? D3D11_SRV_DIMENSION_TEXTURE1D :
			(pTexture->GetMips() == 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = pTexture->GetMips() - 1;
		desc.Texture2D.MipLevels = pTexture->GetMips();

		HRESULT hr = m_pDevice->CreateShaderResourceView(*pTexture->m_pTexture, &desc, pTexture->m_pSRV);
		ibAssertMsg(SUCCEEDED(hr), "Unable to create shader resource view for texture");
	}

	m_pContext->VSSetShaderResources(slot, 1, pTexture->m_pSRV);
}

void ibRenderDevice::PSSetShader(ibRenderShader& pShader)
{
	ComPtr<ID3D11PixelShader> pPS;
	HRESULT hr = pShader.m_pShader->QueryInterface(__uuidof(ID3D11PixelShader), pPS);
	ibAssert(SUCCEEDED(hr));

	m_pContext->PSSetShader(*pPS, NULL, 0);
}

// Rasterizer stage
//void ibRenderDevice::SetViewports(u32 count, ibRenderViewport* pViewports);
//void ibRenderDevice::SetScissors(u32 count, ibRenderScissor* pScissors);

// Render targets (output merger)
void ibRenderDevice::SetRenderTargets(ibRenderTarget* pTargets, ibRenderDepthStencil* pDepthStencil, u32 numViews /*= 0*/)
{
	ID3D11DepthStencilView* pStencilViewRaw = 0;
	if (pDepthStencil)
		pStencilViewRaw = *pDepthStencil->m_view;

	u32 nRawViews = numViews ? numViews : 1;
	ID3D11RenderTargetView** pViewsRaw = new (g_engineHeap, "RT bind array") ID3D11RenderTargetView*[nRawViews];
	for (u32 n = 0; n < nRawViews; ++n)
	{
		pViewsRaw[n] = *pTargets[n].m_view;

#ifdef IB_RENDER_VALIDATE_BOUND_RENDER_TARGET_VEIWS
		ibAssert(pTargets[n].GetWidth() == pTargets->GetWidth());
		ibAssert(pTargets[n].GetHeight() == pTargets->GetHeight());
		ibAssert(pTargets[n].GetMips() == pTargets->GetMips());
		if (pTargets->GetMips() == 1)
		{
			u32 ptSamples, baseSamples;
			u32 ptQuality, baseQuality;
			pTargets[n].GetMSAASettings(&ptSamples, &ptQuality);
			pTargets->GetMSAASettings(&baseSamples, &baseQuality);
			ibAssert(ptSamples == baseSamples);
		}
#endif
	}

#ifdef IB_RENDER_VALIDATE_BOUND_RENDER_TARGET_VEIWS
	if (pDepthStencil)
	{
		ibAssert(pDepthStencil->GetWidth() == pTargets->GetWidth());
		ibAssert(pDepthStencil->GetHeight() == pTargets->GetHeight());
		ibAssert(pDepthStencil->GetMips() == pTargets->GetMips());
		if (pTargets->GetMips() == 1)
		{
			u32 ptSamples, baseSamples;
			u32 ptQuality, baseQuality;
			pDepthStencil->GetMSAASettings(&ptSamples, &ptQuality);
			pTargets->GetMSAASettings(&baseSamples, &baseQuality);
			ibAssert(ptSamples == baseSamples);
		}
	}
#endif

	m_pContext->OMSetRenderTargets(numViews, pViewsRaw, pStencilViewRaw);
	delete [] pViewsRaw;
}

// Batch commit
void ibRenderDevice::Draw(u32 vertexCount, u32 vertexStart /*= 0*/)
{
	m_pContext->Draw(vertexCount, vertexStart);
}

void ibRenderDevice::DrawIndexed(u32 indexCount, u32 start, int vertexOffset /*= 0*/)
{
	m_pContext->DrawIndexed(indexCount, start, vertexOffset);
}
