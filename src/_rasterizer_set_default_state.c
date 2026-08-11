/* _rasterizer_set_default_state @0x8369D778 — reset the D3D pipeline to the rasterizer's default render
 * state at the start of a window: depth test on (LESSEQUAL, writing), no depth bias, stencil off (with
 * the standard ALWAYS/keep configuration), alpha test off (GREATEREQUAL), alpha blend off (SRC/ZERO,
 * ADD), full color write mask, counter-clockwise culling, solid fill. */

#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_DepthBias(D3DDevice *device, int bias);
extern void D3DDevice_SetRenderState_SlopeScaleDepthBias(D3DDevice *device, int bias);
extern void D3DDevice_SetRenderState_StencilEnable(D3DDevice *device, int enable);
extern void D3DDevice_SetRenderState_StencilFail(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_StencilZFail(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_StencilPass(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_StencilFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_StencilRef(D3DDevice *device, int ref);
extern void D3DDevice_SetRenderState_StencilMask(D3DDevice *device, int mask);
extern void D3DDevice_SetRenderState_StencilWriteMask(D3DDevice *device, int mask);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int ref);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_SeparateAlphaBlendEnable(D3DDevice *device, int enable);
extern void D3DDevice_SetRenderState_SrcBlendAlpha(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlendAlpha(D3DDevice *device, int blend);
extern void D3DDevice_SetRenderState_BlendOpAlpha(D3DDevice *device, int op);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_FillMode(D3DDevice *device, unsigned int fill_mode);

void _rasterizer_set_default_state(void)
{
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1u);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 1u);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3u);
    D3DDevice_SetRenderState_DepthBias(global_d3d_device, 0);
    D3DDevice_SetRenderState_SlopeScaleDepthBias(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilFail(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilZFail(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilPass(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilFunc(global_d3d_device, 7u);
    D3DDevice_SetRenderState_StencilRef(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilMask(global_d3d_device, 0);
    D3DDevice_SetRenderState_StencilWriteMask(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaFunc(global_d3d_device, 4u);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1u);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, 0);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
    D3DDevice_SetRenderState_SeparateAlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_SrcBlendAlpha(global_d3d_device, 1u);
    D3DDevice_SetRenderState_DestBlendAlpha(global_d3d_device, 0);
    D3DDevice_SetRenderState_BlendOpAlpha(global_d3d_device, 0);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xFu);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, 6u);
    D3DDevice_SetRenderState_FillMode(global_d3d_device, 0);
}
