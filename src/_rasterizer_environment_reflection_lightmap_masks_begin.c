/* _rasterizer_environment_reflection_lightmap_masks_begin @0x837A6590 — open the reflection lightmap-mask
 * batch: when the relevant debug toggles are on and no lightmap mode is forced, set the D3D pipeline up to
 * accumulate the mask into the alpha channel (cull none, alpha-only color writes, additive alpha blend, depth
 * test on but no depth write) with a clamped, point-filtered sampler 0. Always selects mask shader 36 into
 * dxeffect_shader.
 *
 * DEVIATION: the decompiler threaded rasterizer_shader_select's r3 through to blr and typed the function as
 * returning rasterizer_dx9_shader *; disasm shows r3 is only stored to dxeffect_shader (no return path
 * recomputes it) and no caller consumes it — the thunk discards it. Attested void, matching the whole
 * _rasterizer_*_begin family.
 *
 * DEVIATION: the original inlined the sampler address-mode / separate-Z-filter setters as direct GPU fetch-
 * constant register writes (global_d3d_device->m_Constants.Fetch[0].Texture bit pokes plus m_Pending mask
 * dirties); restored here to the equivalent D3DDevice_SetSamplerState_*_Inline calls (sampler 0, clamp=2). */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/d3d_render_boundary.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/blam_data_globals.h"

extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);

extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
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

void _rasterizer_environment_reflection_lightmap_masks_begin(void)
{
    if (!rasterizer_debug_options.drawing_mode
        && rasterizer_debug_options.draw_environment_reflection_lightmap_masks
        && rasterizer_debug_options.draw_environment_reflections
        && !rasterizer_globals.lightmap_mode)
    {
        D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 8);   /* alpha channel only */
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_DESTALPHA);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_ZERO);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_EQUAL);
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
    }

    dxeffect_shader = rasterizer_shader_select(_dxshader_environment_reflection_lightmap_mask);
}
