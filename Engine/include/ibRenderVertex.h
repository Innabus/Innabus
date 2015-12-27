#ifndef IB_RENDER_VERTEX_H
#define IB_RENDER_VERTEX_H
#pragma once

struct ibVertex2D
{
	float x, y;
};

struct ibVertexP1
{
	float x, y, z;
};

struct ibVertexT1
{
	float x, y, z;
	float u, v;
};

enum ibVertexType {
	kVertex_2D,
	kVertex_P1,
	kVertex_UV,

	kVertex_Count,
};

#endif // IB_RENDER_VERTEX_H
