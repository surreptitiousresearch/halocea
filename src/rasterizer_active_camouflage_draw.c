/* rasterizer_active_camouflage_draw @0x83795010 — draws one transparent-geometry group with the active
 * camouflage (refraction) effect. Runs only while camo is enabled and the primary render target is bound.
 * The expensive path binds the distortion bitmap (sampler 0) and the cached scene render target (sampler 2)
 * through camo effect 105, uploads the refraction scale + tint colour (lerped base->hyper-stealth by the
 * effect parameter) and the view/camera basis as vertex-shader constants, sets the per-pixel intensity as
 * pixel constant c256, and draws the group through the effect. Cheap-camo / no-alpha-target fallbacks skip
 * the refraction pass and just clamp intensity. Either way, when intensity < 1 the model is then drawn
 * normally as a solid overlay faded by (1 - intensity).
 *
 * Sibling of rasterizer_active_camouflage_cache_primary_render_target (same shader-select / sampler-poke /
 * vs-const / effect-pass structure). DEVIATIONS (all disasm-resolved): the inline m_Constants.Fetch pokes
 * are the established named sampler helpers — sampler 0 gets AddressU/V/W_Inline = 1, MagFilter = MinFilter
 * = 1, SeparateZFilterEnable = 1; sampler 2 gets AddressU/V_Inline = 1, MagFilter = MinFilter = 1,
 * SeparateZFilterEnable = 0 (bit fields decoded from the rlwimi MB/ME at 0x480/0x4B0/0x48C/0x4BC).
 * SetVertexShaderConstantFN counts/masks (garbled by Hex-Rays) are li r6 / sldi r7 from disasm: c10 count 3
 * mask 3<<60, c27 count 2 mask 3<<56, c4 count 2 mask 1<<62. The inline Alu[256] poke (four stfs at device
 * 0x1780 + m_Pending mask 1<<63 via std at device 0x8) is pixel constant c256 = intensity, expressed as
 * SetPixelShaderConstantFN per the _rasterizer_decals_draw convention. rasterizer_models_end takes no
 * argument (the decompiler's threaded r3 is a spurious
 * dataflow). The camo colour/flag/distortion fields are now read through named members of
 * game_globals_rasterizer_data; the prior "0x174 tint_color / 0x188 hyper_stealth_tint_color" offset
 * comments were mislabelled — those float runs begin at the refraction_amount / distance_falloff members. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/shader_model_flags.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/real_matrix4x3.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/rasterizer_active_camouflage_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"


#include "headers/vertex_buffer.h"
extern void *memcpy(void *dest, const void *src, unsigned int count);
extern void *memset(void *dest, int value, unsigned int count);

extern void rasterizer_set_frustum_z(float z_near, float z_far);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);
extern void rasterizer_profile_enable(uint8_t enable);
extern void rasterizer_models_begin(uint8_t sky);
extern void rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters, uint8_t do_not_change_z_stencil_states);
extern void rasterizer_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);
extern void rasterizer_model_end(void);
extern void rasterizer_models_end(void);

extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);

void rasterizer_active_camouflage_draw(const transparent_geometry_group *group)
{
    if ( !rasterizer_debug_options.active_camouflage_enabled || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary )
        return;

    float intensity = group->effect.intensity;
    const shader *group_shader = group->shader;

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 )
        rasterizer_set_frustum_z(rasterizer_globals.z_near_first_person, rasterizer_globals.z_far_first_person);

    if ( rasterizer_globals.use_cheap_active_camo || rasterizer_globals.alpha_render_targets_disabled )
    {
        /* cheap / no-alpha-target fallback: skip the refraction pass, just clamp intensity */
        if ( intensity > 0.89999998f )
            intensity = 0.89999998f;
    }
    else
    {
        rasterizer_dx9_shader *dxeffect = rasterizer_shader_select(_dxshader_active_camouflage_draw);
        if ( dxeffect && dxeffect->effect )
        {
            float param = group->effect.parameter;

            D3DDevice_SetVertexDeclaration(global_d3d_device,
                    rasterizer_dx9_shaders_vdecl9_get(rasterizer_transparent_geometry_get_primary_vertex_type(group)));
            D3DDevice_SetRenderState_CullMode(global_d3d_device,
                    /* camouflage groups come from the model submit path: second radiosity block
                     * overlays shader_model.flags (bit 1 = two sided) */
                    (group_shader[1].base.radiosity.flags & (1u << _shader_model_two_sided_bit)) != 0 ? 0 : 6);
            D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
            D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
            D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);

            /* sampler 0 = the distortion bitmap */
            /* recovered: *(int *)(rdata + 0x5C) -> active_camouflage_distortion.index */
            rasterizer_set_texture_direct_for_effect(0, global_rasterizer_data->active_camouflage_distortion.index, 0, dxeffect);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);

            /* sampler 2 = the cached scene render target */
            rasterizer_set_target_as_texture_for_effect(2, 2, 0, dxeffect);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, 1);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, 1);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 0);

            D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_model_active_camouflage));

            int viewport_width = global_window_parameters.camera.viewport_bounds.n[2]
                               - global_window_parameters.camera.viewport_bounds.n[0];
            int viewport_height = global_window_parameters.camera.viewport_bounds.n[3]
                                - global_window_parameters.camera.viewport_bounds.n[1];

            /* camo refraction/falloff/tint lerped base->hyper-stealth by the effect parameter.
             * recovered: (float *)(rdata + 0x174)[0..2] -> refraction_amount, distance_falloff, tint_color.red
             * recovered: (float *)(rdata + 0x188)[0..2] -> hyper_stealth_{refraction_amount, distance_falloff, tint_color.red} */
            float refraction_scale = global_rasterizer_data->active_camouflage_refraction_amount * (1.0f - param)
                                   + global_rasterizer_data->active_camouflage_hyper_stealth_refraction_amount * param;
            float tint_lerp_1 = global_rasterizer_data->active_camouflage_distance_falloff * (1.0f - param)
                              + global_rasterizer_data->active_camouflage_hyper_stealth_distance_falloff * param;
            float tint_lerp_2 = global_rasterizer_data->active_camouflage_tint_color.__s1.red * (1.0f - param)
                              + global_rasterizer_data->active_camouflage_hyper_stealth_tint_color.__s1.red * param;

            /* vertex-shader constants c10..c12: refraction scale/aspect + tint colour */
            float vs_camo[12];
            vs_camo[0] = (1.0f / refraction_scale) * intensity;
            vs_camo[1] = tint_lerp_1;
            vs_camo[2] = global_nvidia_broken_render_targets ? (float)((double)viewport_height * 0.5) : 1.0f;
            vs_camo[3] = global_nvidia_broken_render_targets ? (float)((double)viewport_width * 0.5) : 1.0f;
            vs_camo[4] = 0.0f;
            vs_camo[5] = 0.0f;
            vs_camo[6] = 0.0f;
            vs_camo[7] = 0.0f;
            vs_camo[8] = tint_lerp_2;
            vs_camo[9] = tint_lerp_1;
            vs_camo[10] = tint_lerp_2;
            vs_camo[11] = 0.0f;
            D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, vs_camo, 3, (unsigned __int64)3 << 60);

            /* c27..c28: view->world basis rows 0 and 1 */
            float vs_view_to_world[8];
            vs_view_to_world[0] = global_window_parameters.frustum.view_to_world.n[0][0];
            vs_view_to_world[1] = global_window_parameters.frustum.view_to_world.n[0][1];
            vs_view_to_world[2] = global_window_parameters.frustum.view_to_world.n[0][2];
            vs_view_to_world[3] = 1.0f;
            vs_view_to_world[4] = global_window_parameters.frustum.view_to_world.n[1][0];
            vs_view_to_world[5] = global_window_parameters.frustum.view_to_world.n[1][1];
            vs_view_to_world[6] = global_window_parameters.frustum.view_to_world.n[1][2];
            vs_view_to_world[7] = 3.0f;
            D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x1B, vs_view_to_world, 2, (unsigned __int64)3 << 56);

            /* c4..c5: camera position + forward */
            float vs_camera[8];
            vs_camera[0] = global_window_parameters.camera.position.n[0];
            vs_camera[1] = global_window_parameters.camera.position.n[1];
            vs_camera[2] = global_window_parameters.camera.position.n[2];
            vs_camera[3] = 2.0f;
            vs_camera[4] = global_window_parameters.camera.forward.n[0];
            vs_camera[5] = global_window_parameters.camera.forward.n[1];
            vs_camera[6] = global_window_parameters.camera.forward.n[2];
            vs_camera[7] = 0.5f;
            D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 4, vs_camera, 2, (unsigned __int64)1 << 62);

            unsigned int pass_count[4];
            ID3DXEffect_Begin(dxeffect->effect, pass_count, 3);
            /* recovered: *(unsigned __int16 *)(rdata + 0x170) -> active_camouflage_flags */
            ID3DXEffect_BeginPass(dxeffect->effect, global_rasterizer_data->active_camouflage_flags & (1u << _rasterizer_active_camouflage_tint_edge_density_bit));

            /* pixel-shader constant c256 = intensity across all four channels (inline Alu[256] poke +
             * m_Pending mask 1<<63, per the SetPixelShaderConstantFN convention in _rasterizer_decals_draw) */
            float ps_intensity[4] = { intensity, intensity, intensity, intensity };
            D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 256, ps_intensity, 1, (unsigned __int64)1 << 63);

            rasterizer_transparent_geometry_group_draw_internal(group, 0);
            ID3DXEffect_EndPass(dxeffect->effect);
            ID3DXEffect_End(dxeffect->effect);
        }
    }

    D3DDevice_SetRenderState_ZFunc(global_d3d_device, 2);

    if ( intensity < 1.0f )
    {
        /* second pass: draw the model normally, faded to a solid overlay by (1 - intensity) */
        rasterizer_model_begin_parameters params;
        params.skinning.node_matrices = (real_matrix4x3 *)group->node_matrices;
        params.skinning.node_matrix_count = group->node_matrix_count;
        params.geometry_flags = group->geometry_flags & 0x180;
        params.centroid.n[0] = group->centroid.n[0];
        params.centroid.n[1] = group->centroid.n[1];
        params.centroid.n[2] = group->centroid.n[2];

        /* recovered: 10-dword pointer-walk from &params.animation.values -> memset(&params.effect, ...) */
        memset(&params.effect, 0, sizeof(params.effect));

        params.base_map_scale = group->model_base_map_scale;
        if ( group->lighting )
            memcpy(&params.lighting, group->lighting, sizeof(params.lighting));
        else
            memset(&params.lighting, 0, sizeof(params.lighting));

        if ( group->animation )
            params.animation = *group->animation;
        else
        {
            params.animation.colors = 0;
            params.animation.values = 0;
        }

        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);
        force_alpha = 1.0f - intensity;
        force_alpha_blend = 1;
        rasterizer_profile_enable(0);
        rasterizer_models_begin(0);
        rasterizer_model_begin(&params, 1);
        rasterizer_model_draw(group->shader, group->shader_permutation_index, group->triangle_buffer,
                              group->dynamic_triangle_buffer_index, group->triangle_count,
                              group->vertex_buffers, group->dynamic_vertex_buffer_index);
        rasterizer_model_end();
        rasterizer_models_end();
        rasterizer_profile_enable(1);
        force_alpha_blend = 0;
    }

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 )
        rasterizer_set_frustum_z(0.0f, 0.0f);
}
