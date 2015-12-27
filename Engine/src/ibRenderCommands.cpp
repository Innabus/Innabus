#include "ibRenderCommands.h"

#include "ibRenderDevice.h"

IB_RENDER_FUNCTOR_OP(Clear) { pDevice->ClearImmediate(m_color, m_depth, (u8)(m_stencil & 0xFF), !!(m_stencil & 0xFF00)); }
IB_RENDER_FUNCTOR_OP(BlendSetDesc) { pDevice->BlendSetDesc(m_renderTarget, m_bEnable, m_src, m_dst, m_op, m_srcAlpha, m_dstAlpha, m_opAlpha, m_mask); }
IB_RENDER_FUNCTOR_OP(StencilEnable) { pDevice->StencilEnable(m_bEnable, m_readMask, m_writeMask); }
IB_RENDER_FUNCTOR_OP(StencilFrontFace) { pDevice->StencilFrontFace(m_fail, m_depth, m_pass, m_func); }
IB_RENDER_FUNCTOR_OP(StencilBackFace) {	pDevice->StencilBackFace(m_fail, m_depth, m_pass, m_func); }
IB_RENDER_FUNCTOR_OP(RasterDepthBias) { pDevice->RasterDepthBias(m_bias, m_clamp, m_slope); }
IB_RENDER_FUNCTOR_OP(SetVertexBuffer) { pDevice->SetVertexBuffer(m_slot, m_pBuffer, m_offset); }
IB_RENDER_FUNCTOR_OP(SetRenderTargets) { pDevice->SetRenderTargets(m_pTargets, m_pDepthStencil, m_numViews); }
IB_RENDER_FUNCTOR_OP(DrawIndexed) { pDevice->DrawIndexed(m_count, m_start, m_offset); }
