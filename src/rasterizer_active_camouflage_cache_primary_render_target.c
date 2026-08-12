/* rasterizer_active_camouflage_cache_primary_render_target @0x83794C38 — copy the primary render target into
 * the active-camouflage cache render target (target 2) so refracting/cloaked surfaces can sample the
 * scene behind them. Runs only when active camo is enabled and neither the cheap-camo nor
 * alpha-render-target fallbacks are active. Binds render target 1 as the source texture through effect 96,
 * sets clamp addressing + linear filtering on sampler 0, uploads a screen-space orthographic view matrix
 * (5 vec4 at vertex-shader register 0xD) derived from the current viewport, and draws one full-viewport
 * quad. When the visibility flag is set the copy is performed; either way the stencil mode is restored and
 * the multipass flag is advanced.
 *
 * Sibling of rasterizer_sun_glow_copy_source / _rasterizer_hud_motion_sensor_blip_begin (same
 * shader-select → set-target-as-texture → sampler-poke → ortho-const → full-quad → effect-pass structure).
 *
 * DEVIATIONS (all disasm-resolved): the m_Constants.Fetch[0] pokes are the established inline sampler
 * helpers — the two rlwimi into dword[0] bits 19-21 / 16-18 are AddressU_Inline = 2 / AddressV_Inline = 2
 * (matching _rasterizer_hud_motion_sensor_blip_begin), and dword[3] &= 0xFE7FFFFF is
 * SeparateZFilterEnable = 0; SetVertexShaderConstantFN's count/mask (rendered 0x18000000 / a viewport pun)
 * are 5 and 3<<59 (li r6,5 / sldi r7,3,59); the __int128 viewport packing decodes to plain
 * (float)(int16 viewport_width/height) integer-to-float conversions used to build the ortho matrix. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/blam_data_globals.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int stride);

void rasterizer_active_camouflage_cache_primary_render_target(void)
{
    if ( !rasterizer_debug_options.active_camouflage_enabled
      || rasterizer_globals.use_cheap_active_camo
      || rasterizer_globals.alpha_render_targets_disabled )
        return;

    rasterizer_set_target(2, 0, 0, 0, 0);
    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_none);

    if ( local_active_camouflage_visibility_flag )
    {
        rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_effect_normal_tint);
        D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
        D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));
        rasterizer_set_target_as_texture_for_effect(0, 1, 0, shader);

        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, D3DTADDRESS_CLAMP);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, D3DTEXF_LINEAR);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);

        D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

        float viewport_width = (float)(int16_t)(global_window_parameters.camera.viewport_bounds.n[2]
                - global_window_parameters.camera.viewport_bounds.n[0]);
        float viewport_height = (float)(int16_t)(global_window_parameters.camera.viewport_bounds.n[3]
                - global_window_parameters.camera.viewport_bounds.n[1]);

        /* Screen-space orthographic view matrix (5 vec4 at register 0xD): x maps by 2/width, y by
         * -2/height (disasm 0x83794E68-0x83794EB8). */
        float view_constants[28] = {
            ((1.0f / viewport_width) * 2.0f), 0.0f, 0.0f,
            (-1.0f - (1.0f / viewport_width)),
            0.0f, ((1.0f / viewport_height) * -2.0f), 0.0f,
            ((1.0f / viewport_height) + 1.0f),
            0.0f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
        };
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, view_constants, 5, (uint64_t)3 << 59);

        vertex_data_1[0].position = (real_point3d){ { 0.0f, 0.0f, 0.0f } };
        vertex_data_1[0].texcoord = (real_point2d){ { 0.0f, 0.0f } };
        vertex_data_1[0].color = 0xFFFFFFFF;
        vertex_data_1[1].position = (real_point3d){ { viewport_width, 0.0f, 0.0f } };
        vertex_data_1[1].texcoord = (real_point2d){ { 1.0f, 0.0f } };
        vertex_data_1[1].color = 0xFFFFFFFF;
        vertex_data_1[2].position = (real_point3d){ { viewport_width, viewport_height, 0.0f } };
        vertex_data_1[2].texcoord = (real_point2d){ { 1.0f, 1.0f } };
        vertex_data_1[2].color = 0xFFFFFFFF;
        vertex_data_1[3].position = (real_point3d){ { 0.0f, viewport_height, 0.0f } };
        vertex_data_1[3].texcoord = (real_point2d){ { 0.0f, 1.0f } };
        vertex_data_1[3].color = 0xFFFFFFFF;

        unsigned int pass_count[4];
        ID3DXEffect_Begin(shader->effect, pass_count, 3);
        for ( unsigned int pass = 0; pass < pass_count[0]; ++pass )
        {
            ID3DXEffect_BeginPass(shader->effect, pass);
            D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_1,
                    sizeof(dynamic_screen_vertex));
            ID3DXEffect_EndPass(shader->effect);
        }
        ID3DXEffect_End(shader->effect);
    }

    rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
    local_active_camouflage_debug_cached_flag = 1;
    /* &= (multipass_enabled == 0) - 1 : keep the flag only while multipass is enabled. */
    local_active_camouflage_visibility_flag &=
            (rasterizer_debug_options.active_camouflage_multipass_enabled == 0) - 1;
}
