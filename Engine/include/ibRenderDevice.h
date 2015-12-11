#ifndef IB_RENDER_DEVICE_H
#define IB_RENDER_DEVICE_H
#pragma once

#ifdef IB_WINDOWS
#include <d3d11_1.h>
#endif // IB_WINDOWS

#ifdef IB_WINDOWS
template class IB_RENDER_EXPORT ComPtr<IDXGIFactory1>;
template class IB_RENDER_EXPORT ComPtr<IDXGIAdapter>;
template class IB_RENDER_EXPORT ComPtr<ID3D11Device>;
template class IB_RENDER_EXPORT ComPtr<ID3D11DeviceContext>;
template class IB_RENDER_EXPORT ComPtr<IDXGISwapChain>;
template class IB_RENDER_EXPORT ComPtr<ID3D11RenderTargetView>;
#endif

class IB_RENDER_EXPORT ibRenderDevice
{
public:
	ibRenderDevice();
	~ibRenderDevice();

	void ClearImmediate(const float color[4], float depth, u8 stencil, bool clearState = false);
	void Present();

private:
	void PlatformInit();

#ifdef IB_WINDOWS
	bool CreateDeviceAndContext();
	bool CreateSwapChain();

	ComPtr<IDXGIFactory1> m_pFactory;
	ComPtr<IDXGIAdapter> m_pAdapter;
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

	D3D_FEATURE_LEVEL m_featureLevel;
#endif
};

#endif // IB_RENDER_DEVICE_H
