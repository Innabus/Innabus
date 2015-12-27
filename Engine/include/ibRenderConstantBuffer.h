#ifndef IB_RENDER_CONSTANT_BUFFER_H
#define IB_RENDER_CONSTANT_BUFFER_H
#pragma once

#include "ibRenderBuffer.h"

class IB_RENDER_EXPORT ibRenderConstantBuffer : public ibRenderBuffer
{
public:
private:
#ifdef IB_RENDER_DX11
	ibRenderConstantBuffer(ComPtr<ID3D11Buffer> ptr, ibBufferUsage u):ibRenderBuffer(ptr, u) {}
#endif

	friend class ibRenderDevice;
};

#endif // IB_RENDER_CONSTANT_BUFFER_H
