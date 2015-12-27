#ifndef IB_RENDER_COMMANDS_H
#define IB_RENDER_COMMANDS_H
#pragma once

#include "ibRenderDevice.h"
#include "ibColor.h"

class ibRenderCommand
{
public:
	virtual ~ibRenderCommand() {}
	virtual void operator()(ibRenderDevice*) = 0;
	u32 discriminator;
	u32 size;
};

// Raw name for the render command functor
#define IB_RENDER_FUNCTOR(x) ibRenderCommand_ ## x
// Class declaration for render command functor
#define IB_RENDER_FUNCTOR_DECL(x) class IB_RENDER_FUNCTOR(x) : public ibRenderCommand
// Automatic definition of functor operator
#define IB_RENDER_FUNCTOR_DEFN public: \
	virtual void operator()(ibRenderDevice* pDevice); \
	private:
// Fully tped signature for named functor
#define IB_RENDER_FUNCTOR_OP(x) void IB_RENDER_FUNCTOR(x)::operator()(ibRenderDevice* pDevice)

// Executes a render command on the device with no arguments
#define IB_RENDER_FUNCTOR_COMMAND(x) \
	IB_RENDER_FUNCTOR_DECL(x) { \
	public: \
	void operator()(ibRenderDevice* pd) { pd->x(); } \
}

// Auto-impl; executes a render command on the device with a single bool arg
#define IB_RENDER_FUNCTOR_BOOL(x) \
	IB_RENDER_FUNCTOR_DECL(x) { \
	public: \
	IB_RENDER_FUNCTOR(x)(bool b):m_bVal(b) {} \
	virtual void operator()(ibRenderDevice* pd) { pd->x(m_bVal); } \
	private: \
	bool m_bVal; \
}

// Auto-impl; executes a render command on the device with a single argument of 'type'
#define IB_RENDER_FUNCTOR_TYPED(x, type) \
	IB_RENDER_FUNCTOR_DECL(x) { \
	public: \
	IB_RENDER_FUNCTOR(x)(type t):m_val(t) {} \
	virtual void operator()(ibRenderDevice* pd) { pd->x(m_val); } \
	private: \
	type m_val; \
}

// Auto-impl; executes a render command on the device taking two arguments
#define IB_RENDER_FUNCTOR_TYPED_2(x, type1, type2) \
	IB_RENDER_FUNCTOR_DECL(x) { \
	public: \
	IB_RENDER_FUNCTOR(x)(type1 t1, type2 t2):m_arg1(t1), m_arg2(t2) {} \
	virtual void operator()(ibRenderDevice* pd) { pd->x(m_arg1, m_arg2); } \
	private: \
	type1 m_arg1; \
	type2 m_arg2; \
}

IB_RENDER_FUNCTOR_DECL(Clear) 
{	
public:
	IB_RENDER_FUNCTOR(Clear)(
		const ibColorF& color, 
		const f32 depth,
		const u8 stencil,
		const bool clearState = false):
		m_color(color), m_depth(depth),	m_stencil(stencil)
	{
		m_stencil |= clearState << 8;
	}

	IB_RENDER_FUNCTOR_DEFN;

	ibColorF m_color;
	f32 m_depth;
	u16 m_stencil;
};

IB_RENDER_FUNCTOR_COMMAND(Present);

IB_RENDER_FUNCTOR_COMMAND(BlendReset);
IB_RENDER_FUNCTOR_BOOL(BlendAlphaToCoverage);
IB_RENDER_FUNCTOR_BOOL(BlendIndependent);
IB_RENDER_FUNCTOR_DECL(BlendSetDesc)
{
public:
	IB_RENDER_FUNCTOR(BlendSetDesc)(
		u32 renderTarget,
		bool bEnable,
		ibBlendFactor src,
		ibBlendFactor dst,
		ibBlendOp op,
		ibBlendFactor srcAlpha,
		ibBlendFactor dstAlpha,
		ibBlendOp opAlpha,
		u8 mask):
	m_renderTarget(renderTarget),
	m_bEnable(bEnable),
	m_src(src),
	m_dst(dst),
	m_op(op),
	m_srcAlpha(srcAlpha),
	m_dstAlpha(dstAlpha),
	m_opAlpha(opAlpha),
	m_mask(mask)
	{}

	IB_RENDER_FUNCTOR_DEFN;

	u32 m_renderTarget;
	bool m_bEnable;
	ibBlendFactor m_src;
	ibBlendFactor m_dst;
	ibBlendOp m_op;
	ibBlendFactor m_srcAlpha;
	ibBlendFactor m_dstAlpha;
	ibBlendOp m_opAlpha;
	u8 m_mask;
};
IB_RENDER_FUNCTOR_COMMAND(BlendCommit);

IB_RENDER_FUNCTOR_COMMAND(DepthStencilReset);
IB_RENDER_FUNCTOR_BOOL(DepthEnable);
IB_RENDER_FUNCTOR_TYPED(DepthFunction, ibDepthFunction);
IB_RENDER_FUNCTOR_DECL(StencilEnable)
{
public:
	IB_RENDER_FUNCTOR(StencilEnable)(
		bool bEnable, u8 readMask, u8 writeMask):
	m_bEnable(bEnable),
	m_readMask(readMask),
	m_writeMask(writeMask)
	{}
	IB_RENDER_FUNCTOR_DEFN;

	bool m_bEnable;
	u8 m_readMask;
	u8 m_writeMask;
};
IB_RENDER_FUNCTOR_DECL(StencilFrontFace)
{
public:
	IB_RENDER_FUNCTOR(StencilFrontFace)(
		ibStencilOp fail,
		ibStencilOp depth,
		ibStencilOp pass,
		ibDepthFunction func):
	m_fail(fail), m_depth(depth), m_pass(pass), m_func(func)
	{}
	IB_RENDER_FUNCTOR_DEFN;

	ibStencilOp m_fail, m_depth, m_pass;
	ibDepthFunction m_func;
};
IB_RENDER_FUNCTOR_DECL(StencilBackFace)
{
public:
	IB_RENDER_FUNCTOR(StencilBackFace)(
		ibStencilOp fail,
		ibStencilOp depth,
		ibStencilOp pass,
		ibDepthFunction func):
	m_fail(fail), m_depth(depth), m_pass(pass), m_func(func)
	{}
	IB_RENDER_FUNCTOR_DEFN;

	ibStencilOp m_fail, m_depth, m_pass;
	ibDepthFunction m_func;
};
IB_RENDER_FUNCTOR_TYPED(StencilSetReference, u32);
IB_RENDER_FUNCTOR_COMMAND(DepthStencilCommit);

IB_RENDER_FUNCTOR_COMMAND(RasterReset);
IB_RENDER_FUNCTOR_BOOL(RasterWireframe);
IB_RENDER_FUNCTOR_TYPED_2(RasterCull, ibCullMode, bool);
IB_RENDER_FUNCTOR_DECL(RasterDepthBias)
{
public:
	IB_RENDER_FUNCTOR(RasterDepthBias)(s32 bias, f32 clamp, f32 slope):
		m_bias(bias), m_clamp(clamp), m_slope(slope)
	{}
	IB_RENDER_FUNCTOR_DEFN;
	s32 m_bias;
	f32 m_clamp, m_slope;
};
IB_RENDER_FUNCTOR_BOOL(RasterDepthClip);
IB_RENDER_FUNCTOR_BOOL(RasterScissorEnable);
IB_RENDER_FUNCTOR_BOOL(RasterMultisample);
IB_RENDER_FUNCTOR_BOOL(RasterLineAA);
IB_RENDER_FUNCTOR_COMMAND(RasterCommit);

IB_RENDER_FUNCTOR_DECL(SetVertexBuffer)
{
public:
	IB_RENDER_FUNCTOR(SetVertexBuffer)(
		u32 slot,
		ibRenderVertexBuffer* pBuffer,
		u32 offset):
	m_slot(slot),
	m_pBuffer(pBuffer),
	m_offset(offset)
	{}
	IB_RENDER_FUNCTOR_DEFN;
	u32 m_slot, m_offset;
	ibRenderVertexBuffer* m_pBuffer;
};
IB_RENDER_FUNCTOR_TYPED_2(SetIndexBuffer, ibRenderIndexBuffer*, u32);
IB_RENDER_FUNCTOR_TYPED(SetTopology, ibTopology);

IB_RENDER_FUNCTOR_TYPED_2(VSSetConstantBuffer, u32, ibRenderConstantBuffer*);
IB_RENDER_FUNCTOR_TYPED_2(VSSetSampler, u32, ibRenderSampler*);
IB_RENDER_FUNCTOR_TYPED_2(VSSetResource, u32, ibRenderTexture*);
IB_RENDER_FUNCTOR_TYPED(VSSetShader, ibRenderShader);

IB_RENDER_FUNCTOR_TYPED_2(PSSetConstantBuffer, u32, ibRenderConstantBuffer*);
IB_RENDER_FUNCTOR_TYPED_2(PSSetSampler, u32, ibRenderSampler*);
IB_RENDER_FUNCTOR_TYPED_2(PSSetResource, u32, ibRenderTexture*);
IB_RENDER_FUNCTOR_TYPED(PSSetShader, ibRenderShader);

IB_RENDER_FUNCTOR_DECL(SetRenderTargets)
{
public:
	IB_RENDER_FUNCTOR(SetRenderTargets)(
		ibRenderTarget* pTargets,
		ibRenderDepthStencil* pDepthStencil,
		u32 numViews):
	m_pTargets(pTargets),
	m_pDepthStencil(pDepthStencil),
	m_numViews(numViews)
	{}
	IB_RENDER_FUNCTOR_DEFN;
	ibRenderTarget* m_pTargets;
	ibRenderDepthStencil* m_pDepthStencil;
	u32 m_numViews;
};

IB_RENDER_FUNCTOR_TYPED_2(Draw, u32, u32);
IB_RENDER_FUNCTOR_DECL(DrawIndexed)
{
public:
	IB_RENDER_FUNCTOR(DrawIndexed)(u32 count, u32 start, int offset):
		m_count(count), m_start(start), m_offset(offset)
	{}
	IB_RENDER_FUNCTOR_DEFN;
	u32 m_count, m_start;
	int m_offset;
};

#endif // IB_RENDER_COMMANDS_H