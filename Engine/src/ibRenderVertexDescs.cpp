#include "ibRenderVertexDescs.h"

// Vertex descriptors
D3D11_INPUT_ELEMENT_DESC ibVertex2D_desc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC ibVertexP1_desc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

D3D11_INPUT_ELEMENT_DESC ibVertexT1_desc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ibVertexDesc g_vertexDescs[kVertex_Count] = {
	{ 1, ibVertex2D_desc },
	{ 1, ibVertexP1_desc },
	{ 2, ibVertexT1_desc },
};
