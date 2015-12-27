#ifndef IB_SHADERS_H
#define IB_SHADERS_H
#pragma once

enum ibCommonShaders {
	kCommonShader_VsQuad,
	kCommonShader_PsQuadSolid,

	kCommonShader_Count,
	kCommonShader_Unknown = -2,
	kCommonShader_Invalid = -1
};

// Defined in ibRenderShaderManager.cpp
// g_shaders is used to map from ibCommonShaders values to loaded hash values
extern u32 g_shaders[kCommonShader_Count];

#endif // IB_SHADERS_H
