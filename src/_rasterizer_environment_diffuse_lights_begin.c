/* _rasterizer_environment_diffuse_lights_begin 0x83788F08 — configure the D3D pipeline for the environment
 * diffuse dynamic-light pass: select the diffuse-light effect shader (index 4), bind the distance-attenuation
 * ramp (stage 2) and the normalization cube map (stage 3), set up the four sampler stages, and set additive
 * alpha-tested depth-tested render state. No-op in a special drawing mode, when diffuse lights are disabled, or
 * when the effect shader is unavailable.
 *
 * DEVIATION: the inlined sampler setters were emitted as raw GPU fetch-constant pokes (decompiler "local
 * variable allocation has failed"); restored to the D3DDevice_SetSamplerState_*_Inline calls they inline from.
 * The garbled MagFilter operands are 1 (point) per the disassembly. Stage 2's address mode is hardware-caps
 * conditional: when the device advertises border addressing (D3DCAPS9.TextureAddressCaps bit 3), the border
 * color is reset (D3DDevice_SetSamplerState_BorderColor_Inline value 0 == the dword[5] & 0xFFFFFFFC poke) and
 * U/V/W are set to clamp-to-border (6); otherwise U/V/W are plain clamp (2). */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/d3d_render_boundary.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/blam_data_globals.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);

extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int value);
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
extern void D3DDevice_SetSamplerState_BorderColor_Inline(D3DDevice *device, unsigned int sampler,
                                                         unsigned int value);

void _rasterizer_environment_diffuse_lights_begin(void)
{
    if ( rasterizer_debug_options.drawing_mode
      || !rasterizer_debug_options.draw_environment_diffuse_lights )
        return;

    rasterizer_dx9_shader *dxeffect_shader = rasterizer_shader_select(_dxshader_environment_diffuse_lights);
    if ( !dxeffect_shader || !dxeffect_shader->effect )
        return;

    /* Stage 0 — wrap U/V, point filter, separate-Z on */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_WRAP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);

    /* Stage 1 — clamp U/V/W, point filter, separate-Z on */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 1);

    /* Stage 2 — distance-attenuation ramp; border (if supported) or clamp addressing, separate-Z off */
    rasterizer_set_texture_direct_for_effect(2, global_rasterizer_data->distance_attenuation.index, 0,
                                             dxeffect_shader);
    if ( (global_d3d_caps.TextureAddressCaps & 8) != 0 )
    {
        D3DDevice_SetSamplerState_BorderColor_Inline(global_d3d_device, 2, 0);
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, D3DTADDRESS_BORDER);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, D3DTADDRESS_BORDER);
        D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 2, D3DTADDRESS_BORDER);
    }
    else
    {
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
        D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 2, D3DTADDRESS_CLAMP);
    }
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, D3DTEXF_POINT);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 0);

    /* Stage 3 — normalization cube map, clamp U/V/W, point mag / non-point min, separate-Z off */
    rasterizer_set_texture_direct_for_effect(3, global_rasterizer_data->vector_normalization.index, 0,
                                             dxeffect_shader);
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 3, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, D3DTEXF_POINT);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 3, 0);

    /* Additive, alpha-tested, depth-tested (no z-write) */
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_EQUAL);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);
}
