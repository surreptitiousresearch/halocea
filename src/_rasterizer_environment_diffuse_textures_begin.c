/* _rasterizer_environment_diffuse_textures_begin @0x83789D40 — set up the D3D pipeline for the environment
 * diffuse-texture (base/detail/micro) pass. Pushes additive-or-replace alpha blending (the exact src/dest
 * blend and depth test depend on the special drawing mode), then configures four texture stages (2, 3, 0, 1)
 * with wrap addressing, point min/mag filtering, and the separate-Z filter enabled, and selects stencil mode
 * 5. With diffuse-texture drawing disabled nothing runs.
 *
 * DEVIATION 1: the original inlined the per-stage sampler address-mode / separate-Z-filter setters as direct
 * GPU fetch-constant register writes (the decompiler emitted m_Constants.Fetch[N] / m_Pending.m_Mask pokes and
 * lost the filter values); restored to the equivalent D3DDevice_SetSamplerState_*_Inline calls (address mode
 * cleared to 0 = wrap, separate-Z-filter = 1, point min/mag filter = 1).
 * DEVIATION 2: the ZEnable argument the decompiler rendered as a convoluted arithmetic chain reduces to
 * (drawing_mode != 1). */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_drawing_mode.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
                                                            unsigned int value);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);

void _rasterizer_environment_diffuse_textures_begin(void)
{
    if (!rasterizer_debug_options.draw_environment_textures)
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device,
                                              rasterizer_debug_options.environment_specular_mask_enabled == 0
                                                  ? 7 : 15);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device,
                                      rasterizer_debug_options.drawing_mode != _rasterizer_drawing_mode_overdraw ? 8 : 1);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device,
                                       rasterizer_debug_options.drawing_mode == _rasterizer_drawing_mode_overdraw);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device,
                                     rasterizer_debug_options.drawing_mode != _rasterizer_drawing_mode_overdraw);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, 2);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

    /* Stage 2 */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 1);
    /* Stage 3 */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 3, 1);
    /* Stage 0 */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
    /* Stage 1 */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 1);

    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject_alpha_tested_decal);
}
