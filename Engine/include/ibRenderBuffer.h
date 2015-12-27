#ifndef IB_RENDER_BUFFER_H
#define IB_RENDER_BUFFER_H
#pragma once

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#endif // IB_RENDER_DX11

#include "ibRenderDefs.h"

class IB_RENDER_EXPORT ibRenderBuffer
{
public:
	virtual ~ibRenderBuffer() {}

	bool isValid() const { return *m_pBuffer != 0; }

protected:
	ibBufferUsage m_usage;

#ifdef IB_RENDER_DX11
	ibRenderBuffer(ComPtr<ID3D11Buffer> ptr, ibBufferUsage usage = kUsage_Default):m_pBuffer(ptr), m_usage(usage) {}

	ComPtr<ID3D11Buffer> m_pBuffer;
#endif
};

#endif // IB_RENDER_BUFFER_H
