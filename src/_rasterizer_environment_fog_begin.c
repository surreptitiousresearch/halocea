/* _rasterizer_environment_fog_begin @0x8378FA40 */
/* _rasterizer_environment_fog_begin 0x8378FA40 — compute this frame's eye fog factors and set up the D3D
 * state for the environment fog pass. Computes the camera's signed distance to the fog plane, derives the
 * atmospheric eye-blend (clamped distance/atmospheric_maximum_distance, forced to 1 when the layer-fog flag
 * 0x2 is set) and the planar eye density (clamped -distance/planar_maximum_depth), selects the fog effect,
 * binds the atmospheric and planar density ramp maps, and configures additive alpha-blended, depth-tested
 * (no z-write) rendering. No-op when in a special drawing mode, fog is globally disabled, or the per-pass fog
 * override is off, or the fog effect is unavailable.
 *
 * DEVIATION: the per-sampler address/separate-Z setters were inlined as raw GPU fetch-constant register pokes;
 * restored to D3DDevice_SetSamplerState_*_Inline (stages 0 and 1: clamp U/V, point min/mag, separate-Z). */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/fog_definition_flags.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_shader_helpers.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/d3d_render_boundary.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/blam_data_globals.h"

extern float rasterizer_fog_eye_blend_factor;     /* atmospheric fog eye-blend factor */

extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int ref);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
                                                            unsigned int value);

void _rasterizer_environment_fog_begin(void)
{
    rasterizer_dx9_shader *shader;
    float atmospheric_blend;
    float planar_density;

    if ( rasterizer_debug_options.drawing_mode || !global_fog_enabled || rasterizer_environment_fog_off )
        return;

    shader = rasterizer_shader_select(_dxshader_environment_fog);

    eye_distance_to_fog_plane =
        ((global_window_parameters.fog.plane.n.n[0] * global_window_parameters.camera.position.n[0])
              + ((global_window_parameters.camera.position.n[1] * global_window_parameters.fog.plane.n.n[1])
                      + (global_window_parameters.camera.position.n[2] * global_window_parameters.fog.plane.n.n[2])))
        - global_window_parameters.fog.plane.d;

    atmospheric_blend = (eye_distance_to_fog_plane / global_window_parameters.fog.atmospheric_maximum_distance);
    if ( atmospheric_blend >= 0.0f )
        rasterizer_fog_eye_blend_factor = atmospheric_blend <= 1.0f ? atmospheric_blend : 1.0f;
    else
        rasterizer_fog_eye_blend_factor = 0.0f;

    planar_density = -(eye_distance_to_fog_plane / global_window_parameters.fog.planar_maximum_depth);
    if ( planar_density >= 0.0f )
        planar_eye_density = planar_density <= 1.0f ? planar_density : 1.0f;
    else
        planar_eye_density = 0.0f;

    if ( (global_window_parameters.fog.fog_definition_flags & (1u << _fog_definition_atmosphere_dominant_bit)) != 0 )
        rasterizer_fog_eye_blend_factor = 1.0f;

    if ( !shader || !shader->effect )
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_EQUAL);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

    rasterizer_set_texture_direct_for_effect(0, global_rasterizer_data->atmospheric_fog_density.index, 0, shader);
    /* Stage 0 — clamp U/V, point filter, separate-Z */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);

    rasterizer_set_texture_direct_for_effect(1, global_rasterizer_data->planar_fog_density.index, 0, shader);
    /* Stage 1 — clamp U/V, point filter, separate-Z */
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 1);

    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVSRCALPHA);
}
