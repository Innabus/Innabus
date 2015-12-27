#ifndef IB_VERTEX_DESCS_H
#define IB_VERTEX_DESCS_H
#pragma once 

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>
#include "ibRenderVertex.h"

struct ibVertexDesc
{
	u32 elemCount;
	D3D11_INPUT_ELEMENT_DESC* desc;
} extern g_vertexDescs[kVertex_Count];

#endif

#endif // IB_VERTEX_DESCS_H
