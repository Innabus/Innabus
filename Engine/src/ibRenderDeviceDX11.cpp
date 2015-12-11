#include "ibRenderDevice.h"

#include "ibLog.h"
#include "ibSystem.h"

#include "DXGI.h"

namespace
{
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
