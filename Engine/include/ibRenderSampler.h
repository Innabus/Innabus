#ifndef IB_RENDER_SAMPLER_H
#define IB_RENDER_SAMPLER_H
#pragma once

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#endif // IB_RENDER_DX11

#include "ibRenderDefs.h"

class IB_RENDER_EXPORT ibRenderSampler
{
public:
	bool isValid() const { return *m_pSampler != 0; }
private:
#ifdef IB_RENDER_DX11
	ibRenderSampler(ComPtr<ID3D11SamplerState>& s):m_pSampler(s) {}
	ComPtr<ID3D11SamplerState> m_pSampler;
#endif // IB_RENDER_DX11

	friend class ibRenderDevice;
};

#endif // IB_RENDER_SAMPLER_H
