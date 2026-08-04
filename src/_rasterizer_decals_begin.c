/* _rasterizer_decals_begin @0x83711084 — set up the D3D pipeline for a decal layer. Records the layer, resets
 * the decal batch caches, binds a clamped point-sampled stage 0, and configures additive alpha blending with
 * depth test on / depth write off plus the decal z-bias. Layer 3 (the alpha-tested overlay layer) uses a 0x7F
 * alpha reference and stencil mode 4; other layers enable alpha testing only when the "filthy" decal fog hack
 * is active (debug option set and the window's atmospheric fog at full density). With decals disabled only
 * layer 3 is processed; nothing runs in special drawing modes.
 *
 * DEVIATION: the original inlined the sampler address-mode / separate-Z-filter setters as direct GPU fetch-
 * constant register writes; restored to the equivalent D3DDevice_SetSamplerState_*_Inline calls (sampler 0). */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/point2d.h"
#include "headers/decal_layer.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern __int16 rasterizer_decal_cached_bitmap_index;
extern int rasterizer_decal_cached_bitmap_group_index;

extern point2d *rasterizer_set_texture(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);
extern void rasterizer_dx9_set_decal_zbias(void);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
                                                            unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int ref);

void _rasterizer_decals_begin(int16_t layer)
{
    unsigned char enable_alpha_test = 1;
    int proceed = 1;

    local_layer = layer;
    if (!rasterizer_debug_options.draw_environment_decals)
        proceed = (layer == _decal_layer_alpha_tested);

    if (rasterizer_debug_options.drawing_mode || !proceed)
        return;

    local_framebuffer_blend_function = -1;
    rasterizer_decal_cached_bitmap_index = -1;
    rasterizer_decal_cached_bitmap_group_index = -1;
    local_filthy_decal_fog_hack_enabled = 0;

    rasterizer_set_texture(0, 0, 1, -1, 0);
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 2);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 2);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
    rasterizer_dx9_set_decal_zbias();

    if (layer == _decal_layer_alpha_tested)
    {
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7F);
        rasterizer_set_stencil_mode(_rasterizer_stencil_mode_write_alpha_tested_decal);
    }
    else
    {
        if (rasterizer_debug_options.filthy_decal_fog_hack_enabled
            && global_window_parameters.fog.atmospheric_maximum_density == 1.0f)
            local_filthy_decal_fog_hack_enabled = 1;
        else
            enable_alpha_test = local_filthy_decal_fog_hack_enabled;

        if (enable_alpha_test)
        {
            D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
        }
        else
        {
            D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        }
    }
}
