#ifndef IB_RENDER_INDEX_BUFFER_H
#define IB_RENDER_INDEX_BUFFER_H
#pragma once

#include "ibRenderBuffer.h"

enum ibIndexFormat {
	kIndex_16,
	kIndex_32,

	kIndex_Count
};

class IB_RENDER_EXPORT ibRenderIndexBuffer : public ibRenderBuffer
{
public:
	ibIndexFormat GetFormat() const { return m_format; }

private:
	ibIndexFormat m_format;

#ifdef IB_RENDER_DX11
	ibRenderIndexBuffer(ComPtr<ID3D11Buffer> ptr, ibBufferUsage u, ibIndexFormat fmt):ibRenderBuffer(ptr, u), m_format(fmt) {}
#endif

	friend class ibRenderDevice;
};

#endif // IB_RENDER_INDEX_BUFFER_H
