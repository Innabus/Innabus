#ifndef IB_RENDER_SHADER_H
#define IB_RENDER_SHADER_H
#pragma once

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#endif // IB_RENDER_DX11

#include "ibRenderDefs.h"

class IB_RENDER_EXPORT ibRenderShader
{
public:
	bool isValid() const { return *m_pShader != 0; }

private:
	ibShaderType m_type;

#ifdef IB_RENDER_DX11
	ibRenderShader(ibShaderType type, ComPtr<ID3D11DeviceChild> ptr):m_type(type), m_pShader(ptr) {}

	ComPtr<ID3D11DeviceChild> m_pShader;
#endif

	friend class ibRenderDevice;
};

#endif // IB_RENDER_SHADER_H
