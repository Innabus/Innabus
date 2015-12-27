#ifndef IB_RENDER_DEFS_H
#define IB_RENDER_DEFS_H
#pragma once

enum ibBlendFactor {
	kBlend_Zero,
	kBlend_One,
	kBlend_SrcColor,
	kBlend_InvSrcColor,
	kBlend_SrcAlpha,
	kBlend_InvSrcAlpha,
	kBlend_DestAlpha,
	kBlend_InvDestAlpha,
	kBlend_DestColor,
	kBlend_InvDestColor,
	
	kBlend_Count
};

enum ibBlendOp {
	kBlendOp_Add,
	kBlendOp_Sub,
	kBlendOp_RevSub,
	kBlendOp_Min,
	kBlendOp_Max,

	kBlendOp_Count
};

enum ibWriteEnable {
	kWriteEnable_Red = 1,
	kWriteEnable_Green = 2,
	kWriteEnable_Blue = 4,
	kWriteEnable_Alpha = 8,
	kWriteEnable_All = 15,
};

enum ibCompareFunc {
	kCompare_Never,
	kCompare_Less,
	kCompare_Equal,
	kCompare_LessEqual,
	kCompare_Greater,
	kCompare_NotEqual,
	kCompare_GreaterEqual,
	kCompare_Always,

	kCompare_Count
};
typedef ibCompareFunc ibDepthFunction;
typedef ibCompareFunc ibFilterCompare;

enum ibStencilOp {
	kStencil_Keep,
	kStencil_Zero,
	kStencil_Replace,
	kStencil_Incr,
	kStencil_Decr,
	kStencil_Invert,
	kStencil_IncrWrap,
	kStencil_DecrWrap,

	kStencil_Count
};

enum ibCullMode {
	kCull_None,
	kCull_Front,
	kCull_Back,

	kCull_Count
};

enum ibTextureFilter
{
	kFilter_Point,
	kFilter_Bilinear,
	kFilter_Trilinear,
	kFilter_Anisotropic,

	kFilter_Count
};

enum ibTextureAddressMode
{
	kTexture_Wrap,
	kTexture_Mirror,
	kTexture_Clamp,
	kTexture_Border,

	kTexture_Count
};

enum ibTopology {
	kTopology_Point,
	kTopology_Line,
	kTopology_LineStrip,
	kTopology_Triangle,
	kTopology_TriStrip,

	kTopology_Count
};

enum ibShaderType {
	kShader_Compute,
	kShader_Domain,
	kShader_Hull,
	kShader_Geometry,
	kShader_Pixel,
	kShader_Vertex,

	kShader_Count
};

enum ibUsage {
	kUsage_Default,
	kUsage_Immutable,
	kUsage_Dynamic,
	kUsage_Staging,

	kUsage_Count
};

typedef ibUsage ibBufferUsage;
typedef ibUsage ibTextureUsage;

enum ibFormat {
	kFormat_RGBA,    // R8G8B8A8_UNORM
	kFormat_R8,      // R8_UNORM
	kFormat_DXT1,    // DXT1/BC1_UNORM
	kFormat_DXT2,    // DXT2/BC2_UNORM
	kFormat_DXT3,    // DXT3/BC3_UNORM
	kFormat_DXT4,    // DXT4/BC4_UNORM
	kFormat_DXT5,    // DXT5/BC5_UNORM
	kFormat_F16,     // R16_FLOAT
	kFormat_F32,     // R32_FLOAT
	kFormat_Depth,   // D24_UNORM_S8_UINT / R24G5_TYPELESS
	kFormat_Depth32, // D32_FLOAT_S8X24_UINT / R32G8X24_TYPELESS

	kFormat_Count
};

enum ibTextureType {
	kTextureDim_1D,
	kTextureDim_2D,

	kTextureType_Count
};

#ifdef IB_RENDER_DX11
#include <d3d11_1.h>

template class IB_RENDER_EXPORT ComPtr<IDXGIFactory1>;
template class IB_RENDER_EXPORT ComPtr<IDXGIAdapter>;
template class IB_RENDER_EXPORT ComPtr<ID3D11Device>;
template class IB_RENDER_EXPORT ComPtr<ID3D11DeviceContext>;
template class IB_RENDER_EXPORT ComPtr<IDXGISwapChain>;
template class IB_RENDER_EXPORT ComPtr<ID3D11RenderTargetView>;
template class IB_RENDER_EXPORT ComPtr<ID3D11BlendState>;
template class IB_RENDER_EXPORT ComPtr<ID3D11DepthStencilState>;
template class IB_RENDER_EXPORT ComPtr<ID3D11RasterizerState>;
template class IB_RENDER_EXPORT ComPtr<ID3D11Buffer>;
template class IB_RENDER_EXPORT ComPtr<ID3D11SamplerState>;
template class IB_RENDER_EXPORT ComPtr<ID3D11DeviceChild>;
template class IB_RENDER_EXPORT ComPtr<ID3D11Resource>;
template class IB_RENDER_EXPORT ComPtr<ID3D11ShaderResourceView>;
#endif

#endif // IB_RENDER_DEFS_H
