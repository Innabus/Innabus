#ifndef IB_RENDER_DEPTH_STENCIL_H
#define IB_RENDER_DEPTH_STENCIL_H
#pragma once

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#endif

#include "ibRenderTexture.h"
#include "ibRenderDefs.h"

class ibRenderDepthStencil
{
public:
	ibRenderDepthStencil():m_view(0), m_msaaSamples(0), m_msaaQuality(0) {}

	u32 GetWidth() const { return m_texture.GetWidth(); }
	u32 GetHeight() const { return m_texture.GetHeight(); }
	ibFormat GetFormat() const { return m_texture.GetFormat(); }
	u32 GetMips() const { return m_texture.GetMips(); }

	void GetMSAASettings(u32* pSamples, u32* pQuality) const {
		ibAssert(pSamples && pQuality);
		*pSamples = m_msaaSamples;
		*pQuality = m_msaaQuality;
	}

	bool isValid() const { return m_texture.isValid() && *m_view != 0; }

private:
	ibRenderTexture m_texture;

#ifdef IB_RENDER_DX11
	ibRenderDepthStencil(ibRenderTexture tex, ComPtr<ID3D11DepthStencilView> view, u32 samples, u32 quality):
		m_texture(tex), m_view(view), m_msaaSamples(samples), m_msaaQuality(quality)
	{}

	ComPtr<ID3D11DepthStencilView> m_view;
	u32 m_msaaSamples;
	u32 m_msaaQuality;
#endif

	friend class ibRenderDevice;
};

#endif // IB_RENDER_DEPTH_STENCIL_H
