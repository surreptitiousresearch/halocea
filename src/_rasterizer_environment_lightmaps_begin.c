/* _rasterizer_environment_lightmaps_begin @0x8378ECE0 */
/* _rasterizer_environment_lightmaps_begin 0x8378ECE0 — configure the D3D pipeline for the environment
 * lightmap pass: back-face cull, full color write, opaque (no alpha blend), z-test less-equal with z-writes,
 * then per-sampler addressing/filtering for the four lightmap-related stages. With lightmap drawing disabled
 * nothing runs.
 *
 * DEVIATION: the decompiler flagged this function ("local variable allocation has failed") and emitted the
 * inlined per-sampler setters as raw GPU fetch-constant register pokes (m_Constants.Fetch[N].Texture /
 * m_Pending.m_Mask) with the overlapping-temp artifacts that implies. Restored to the equivalent
 * D3DDevice_SetSamplerState_*_Inline calls decoded from those register writes: stage 1 wrap U/V; stage 2
 * clamp U/V; stage 3 clamp U/V/W; stage 0 wrap U/V; all with point min/mag filtering and the separate-Z
 * filter enabled (stages 2,3,0). Address-mode field values: 0 = wrap, 2 = clamp. */

#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_render_boundary.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/blam_data_globals.h"


extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int ref);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
                                                            unsigned int value);

void _rasterizer_environment_lightmaps_begin(void)
{
    if ( !rasterizer_debug_options.draw_environment_lightmaps )
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xF);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7F);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_LESSEQUAL);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 1);

    /* Stage 1 — lightmap: wrap U/V */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, D3DTADDRESS_WRAP);
    /* Stage 2 — clamp U/V, point filter, separate-Z */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 1);
    /* Stage 3 — clamp U/V/W, point filter, separate-Z */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 3, 1);
    /* Stage 0 — wrap U/V, point filter, separate-Z */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
}
