#ifndef IB_RENDER_TEXTURE_H
#define IB_RENDER_TEXTURE_H
#pragma once

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#endif

#include "ibRenderDefs.h"

class IB_RENDER_EXPORT ibRenderTexture
{
public:
	ibRenderTexture() { m_pTexture = 0; m_pSRV = 0; }

	u32 GetWidth() const { return m_width; }
	u32 GetHeight() const { return m_height; }
	ibFormat GetFormat() const { return m_format; }
	ibTextureType GetType() const { return m_type; }
	u32 GetMips() const { return m_mips; }

	bool isValid() const { return *m_pTexture != 0; }

private:
	u32 m_width;
	u32 m_height;
	u32 m_mips;
	ibFormat m_format;
	ibTextureType m_type;

#ifdef IB_RENDER_DX11
	ibRenderTexture(
		ibTextureType type,
		u32 width, u32 height,
		ibFormat format,
		u32 mips,
		ComPtr<ID3D11Resource> pTex):
	m_width(width), 
	m_height(height),
	m_format(format),
	m_mips(mips),
	m_pTexture(pTex),
	m_type(type)
	{}

	ComPtr<ID3D11Resource> m_pTexture;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
#endif

	friend class ibRenderDevice;
};

#endif // IB_RENDER_TEXTURE_H
