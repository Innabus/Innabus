#ifndef IB_RENDER_VERTEX_BUFFER_H
#define IB_RENDER_VERTEX_BUFFER_H
#pragma once

#include "ibRenderBuffer.h"

class IB_RENDER_EXPORT ibRenderVertexBuffer : public ibRenderBuffer
{
public:
	u32 GetStride() const { return m_stride; }

private:
	u32 m_stride;

#ifdef IB_RENDER_DX11
	ibRenderVertexBuffer(ComPtr<ID3D11Buffer> ptr, u32 stride, ibBufferUsage u):ibRenderBuffer(ptr, u), m_stride(stride) {}
#endif

	friend class ibRenderDevice;
};

#endif // IB_RENDER_VERTEX_BUFFER_H
