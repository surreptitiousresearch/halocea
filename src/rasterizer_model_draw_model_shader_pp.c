/* rasterizer_model_draw_model_shader_pp @0x8378BF80 — the pixel-processor (post-2.0-shader) draw path for a
 * model "shader_model" (base/detail/multipurpose/reflection + self-illumination + change-color) tag. Sets the
 * fixed render state for opaque/alpha-tested/decal geometry, computes the reflection fade and per-draw self-
 * illumination and change colours, selects the vertex-shader variant (25/26/28/29) from fog/light/skinning/map
 * state, binds the four maps into the effect, uploads the self-illumination, change-colour and per-draw fog
 * constant vectors, builds the animated base-map + detail-map UV transform and the reflection-tint block as
 * vertex-shader constants, then draws the dynamic triangles once per effect pass. Two-sided shaders that were
 * culled on the first pass get a second back-face pass with the detail-map transform.
 *
 * Fields are read through the DB `shader_model` wrapper's nested `->model.<field>` member (the common
 * base is `->shader`); see headers/shader_model.h. The reads are a clean, type-driven decompile.
 *
 * DEVIATIONS (verified against disasm 0x8378C8EC-0x8378CB1C):
 *  - shader_texture_animation_evaluate's two output rows are written through &model_vertex_constants[4] (u) and
 *    &model_vertex_constants[8] (v); the decompiler dropped the address-of (ABI: the 9th/10th args spill to the
 *    outgoing stack area because the six float args skip r5-r10). The inline presets to [4..11] are the identity
 *    fallback that the evaluate call overwrites; translucency (index 10) is re-applied afterwards.
 *  - both D3DDevice_SetVertexShaderConstantFN count/mask pairs the decompiler garbled are (0xA, .., 3, 3<<60)
 *    and (0xD, .., 2, 1<<60) — li r6,3/sldi r7,r7,60 and li r6,2/extldi r7,r7,64,60 (=1<<60). The ambient
 *    reflection tint upload is likewise (0xD, .., 2, 1<<60).
 *  - the shader-no-fog-flag (0x10) branch of the fog block reads the four contiguous change-colour stack slots;
 *    the 4th (v105 @ sp+7C) is never written by this function (a shipped uninitialized read) — reproduced as 0
 *    for its fog_delta_b term, per the corpus convention (cf. rasterizer_glass_draw_diffuse_pp DEVIATION 3).
 *  - self-illumination colours are unused when detail_mask is set (that branch never uploads constants[4] and
 *    the later gate is `!detail_mask`); they are initialised to 0 here to avoid an uninitialised read. */

#include "headers/fog_definition_flags.h"
#include "headers/shader_model.h"
#include "headers/vertex_buffer.h"
#include "headers/triangle_buffer.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_frame_begin_parameters.h"
#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"
#include "headers/real_vector4d.h"
#include "headers/render_animation.h"
#include "headers/shader_texture_animation.h"
#include "headers/ppc_intrinsics.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_model_flags.h"
#include "headers/shader_model_self_illumination_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"

#include "headers/shader_texture_animation.h"
#include "headers/render_animation.h"
#include "headers/point2d.h"
extern real_argb_color *global_rasterizer_model_ambient_reflection_tint;

extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);

extern rasterizer_dx9_shader * rasterizer_model_setup_model_shader(shader_model *shader_model);
extern void rasterizer_dx9_set_decal_zbias(void);
extern void rasterizer_dx9_reset_zbias(void);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
#include <stdint.h>
extern float real_seed_random(uint32_t *seed);
extern float periodic_function_evaluate(int16_t function_type, float time);
extern void shader_texture_animation_evaluate(const shader_texture_animation *texture_animation, const render_animation *render_animation, float u_scale, float v_scale, float u_offset, float v_offset, float r_offset, float time_value, real_vector4d *u_transform_reference, real_vector4d *v_transform_reference);
extern void rasterizer_draw(const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);

void rasterizer_model_draw_model_shader_pp(const shader *shader_base, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index)
{
    /* dispatch slot passes the generic shader*; this impl handles the shader_model derived
       layout — downcast here (sibling _environment_shader_pp shape) instead of a funcptr
       cast at the table store */
    shader_model *shader = (shader_model *)shader_base;
    uint8_t decal_flag = (shader->model.flags >> _shader_model_alpha_blended_decal_bit) & 1;

    rasterizer_dx9_shader *effect_shader = rasterizer_model_setup_model_shader(shader);
    if ( !effect_shader || !effect_shader->effect )
        return;

    bool cull_enabled = true;

    float camera_relative_x = local_parameters->centroid.n[0] - global_window_parameters.camera.position.n[0];
    float camera_relative_y = local_parameters->centroid.n[1] - global_window_parameters.camera.position.n[1];
    float camera_relative_z = local_parameters->centroid.n[2] - global_window_parameters.camera.position.n[2];

    /* view-space depth of the centroid (dot of camera forward with centroid-relative-to-camera) */
    float view_depth = global_window_parameters.camera.forward.n[1] * camera_relative_y
            + (camera_relative_x * global_window_parameters.camera.forward.n[0]
                + global_window_parameters.camera.forward.n[2] * camera_relative_z);

    /* reflection fade fraction (0..1) across the cutoff..falloff distance band */
    float reflection_fraction;
    if ( shader->model.reflection_cutoff_distance == 0.0f )
    {
        reflection_fraction = 1.0f;
    }
    else
    {
        float reflection_band = (view_depth - shader->model.reflection_cutoff_distance)
                / (shader->model.reflection_falloff_distance - shader->model.reflection_cutoff_distance);
        if ( reflection_band < 0.0f )
            reflection_fraction = 0.0f;
        else if ( reflection_band > 1.0f )
            reflection_fraction = 1.0f;
        else
            reflection_fraction = reflection_band;
    }

    /* depth state: no-z-test geometry, else z-test with optional decal z-bias */
    if ( local_parameters->geometry_flags & (1u << _rasterizer_geometry_no_zbuffer_bit) )
    {
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
    }
    else
    {
        if ( !force_alpha_blend )
        {
            D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
            D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, decal_flag == 0);
        }
        if ( decal_flag )
            rasterizer_dx9_set_decal_zbias();
        else
            rasterizer_dx9_reset_zbias();
    }

    unsigned __int16 shader_flags = shader->model.flags;
    if ( shader_flags & (1u << _shader_model_two_sided_bit) )
    {
        /* two-sided shader: keep culling only when the nocull flag is clear, or the centroid is within 8 units */
        cull_enabled = (shader_flags & (1u << _shader_model_nocull_two_sided_bit)) == 0
                || __fsqrts(camera_relative_x * camera_relative_x
                        + (camera_relative_z * camera_relative_z + camera_relative_y * camera_relative_y)) <= 8.0f;
    }
    bool cull_enabled_first_pass = cull_enabled;
    D3DDevice_SetRenderState_CullMode(global_d3d_device, !cull_enabled ? 0 : 6);

    if ( !force_alpha_blend )
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);

    unsigned int alpha_blend_enable = 0;
    if ( decal_flag || force_alpha_blend )
        alpha_blend_enable = 1;
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, alpha_blend_enable);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 6);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, 7);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);

    unsigned int alpha_test_enable = 1;
    if ( force_alpha_blend || decal_flag || (shader->model.flags & (1u << _shader_model_not_alpha_tested_bit)) )
        alpha_test_enable = 0;
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, alpha_test_enable);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7F);

    rasterizer_set_texture_for_effect(0, 0, 1, shader->model.base_map.index, shader_permutation_index, effect_shader);
    rasterizer_set_texture_for_effect(1, 0, 2, shader->model.detail_map.index, shader_permutation_index, effect_shader);
    rasterizer_set_texture_for_effect(2, 0, 1, shader->model.multipurpose_map.index, shader_permutation_index,
            effect_shader);
    rasterizer_set_texture_for_effect(3, 2, 0, shader->model.reflection_map.index, shader_permutation_index,
            effect_shader);

    /* per-draw self-illumination colour (unused / not uploaded when detail_mask is set) */
    float self_illum_r = 0.0f, self_illum_g = 0.0f, self_illum_b = 0.0f;
    if ( !shader->model.detail_mask )
    {
        int self_illum_use_fixed_phase = shader->model.self_illumination_flags & (1u << _shader_model_self_illumination_no_random_phase_bit);
        unsigned int self_illum_seed[2];
        self_illum_seed[0] = local_parameters->unique_id;

        double self_illum_phase = self_illum_use_fixed_phase ? 0.0 : real_seed_random(self_illum_seed);
        float self_illum_range_r = shader->model.self_illumination_animation_color_upper_bound.n[0]
                - shader->model.self_illumination_animation_color_lower_bound.n[0];
        float self_illum_range_g = shader->model.self_illumination_animation_color_upper_bound.n[1]
                - shader->model.self_illumination_animation_color_lower_bound.n[1];
        float self_illum_range_b = shader->model.self_illumination_animation_color_upper_bound.n[2]
                - shader->model.self_illumination_animation_color_lower_bound.n[2];
        self_illum_phase += *(double *)&global_frame_parameters.game_time_sec
                / shader->model.self_illumination_animation_period;
        float self_illum_function_value =
                periodic_function_evaluate(shader->model.self_illumination_animation_function, self_illum_phase);

        int self_illumination_color_source = shader->model.self_illumination_color_source;
        self_illum_r = self_illum_range_r * self_illum_function_value
                + shader->model.self_illumination_animation_color_lower_bound.n[0];
        self_illum_g = self_illum_range_g * self_illum_function_value
                + shader->model.self_illumination_animation_color_lower_bound.n[1];
        self_illum_b = self_illum_range_b * self_illum_function_value
                + shader->model.self_illumination_animation_color_lower_bound.n[2];
        if ( self_illumination_color_source > 0 && self_illumination_color_source < 5 )
        {
            const real_rgb_color *source_color =
                    &local_parameters->animation.colors[self_illumination_color_source - 1];
            self_illum_r *= source_color->n[0];
            self_illum_g *= source_color->n[1];
            self_illum_b *= source_color->n[2];
        }

        if ( effect_shader->constants )
        {
            D3DXVECTOR4 self_illum_vector;
            self_illum_vector.x = self_illum_r;
            self_illum_vector.y = self_illum_g;
            self_illum_vector.z = self_illum_b;
            self_illum_vector.w = 1.0f;
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[4], &self_illum_vector);
        }
    }

    /* per-draw change / diffuse colour (white when the shader has no change-colour source) */
    float change_color_r, change_color_g, change_color_b;
    int diffuse_change_color_source = shader->model.diffuse_change_color_source;
    if ( diffuse_change_color_source <= 0 || diffuse_change_color_source >= 5 )
    {
        change_color_r = global_real_rgb_white->n[0];
        change_color_g = global_real_rgb_white->n[1];
        change_color_b = global_real_rgb_white->n[2];
    }
    else
    {
        const real_rgb_color *source_color = &local_parameters->animation.colors[diffuse_change_color_source - 1];
        change_color_r = source_color->n[0];
        change_color_g = source_color->n[1];
        change_color_b = source_color->n[2];
    }

    /* vertex-shader variant selection */
    __int16 vertex_shader_index;
    if ( shader_flags & (1u << _shader_model_true_atmospheric_fog_bit) )
    {
        vertex_shader_index = 28;
    }
    else if ( local_planar_fog_flag )
    {
        vertex_shader_index = 25;
    }
    else if ( local_parameters->lighting.point_light_count > 0 )
    {
        vertex_shader_index = 26;
    }
    else if ( local_parameters->skinning.node_matrix_count <= 1
            && (shader->model.multipurpose_map.index == -1
                || (!shader->model.detail_mask && self_illum_r == 0.0f && self_illum_g == 0.0f && self_illum_b == 0.0f
                    && change_color_r == 1.0f && change_color_g == 1.0f && change_color_b == 1.0f))
            && (shader->model.reflection_map.index == -1 || reflection_fraction <= 0.0f) )
    {
        vertex_shader_index = 29;
    }
    else
    {
        vertex_shader_index = 28;
    }

    /* per-draw fog constants (planar/atmospheric blend, or the change colour passed through for a no-fog shader) */
    float fog_alpha;
    float fog_delta_r, fog_delta_g, fog_delta_b;
    float fog_clamped_r, fog_clamped_g, fog_clamped_b;
    float fog_atmos_r, fog_atmos_g, fog_atmos_b;
    unsigned char fog_computed = 0;

    if ( global_fog_enabled )
    {
        if ( shader_flags & (1u << _shader_model_true_atmospheric_fog_bit) )
        {
            /* no-fog shader variant: pass the change colour straight through. The 4th slot (fog_delta_b) is an
             * uninitialised stack read in the original; reproduced as 0. */
            fog_alpha = change_color_r;
            fog_delta_r = change_color_g;
            fog_delta_g = change_color_b;
            fog_delta_b = 0.0f;
            fog_clamped_r = change_color_r;
            fog_clamped_g = change_color_g;
            fog_clamped_b = change_color_b;
            fog_atmos_r = change_color_r;
            fog_atmos_g = change_color_g;
            fog_atmos_b = change_color_b;
            fog_computed = 1;
        }
        else if ( (local_parameters->geometry_flags & (1u << _rasterizer_geometry_no_fog_bit)) == 0 )
        {
            float plane_distance = (global_window_parameters.fog.plane.n.n[0]
                        * global_window_parameters.camera.position.n[0]
                    + (global_window_parameters.fog.plane.n.n[2] * global_window_parameters.camera.position.n[2]
                        + global_window_parameters.fog.plane.n.n[1] * global_window_parameters.camera.position.n[1]))
                    - global_window_parameters.fog.plane.d;
            float planar_blend = plane_distance / global_window_parameters.fog.atmospheric_maximum_distance;
            if ( planar_blend < 0.0f )
                planar_blend = 0.0f;
            else if ( planar_blend > 1.0f )
                planar_blend = 1.0f;

            float atmospheric_blend = (view_depth - global_window_parameters.fog.atmospheric_minimum_distance)
                    / (global_window_parameters.fog.atmospheric_maximum_distance
                        - global_window_parameters.fog.atmospheric_minimum_distance);
            if ( atmospheric_blend < 0.0f )
                atmospheric_blend = 0.0f;
            else if ( atmospheric_blend > 1.0f )
                atmospheric_blend = 1.0f;

            if ( global_window_parameters.fog.fog_definition_flags & (1u << _fog_definition_atmosphere_dominant_bit) )
                planar_blend = 1.0f;

            float atmospheric_density = atmospheric_blend * global_window_parameters.fog.atmospheric_maximum_density;
            fog_alpha = 1.0f - atmospheric_density;

            fog_delta_r = -(((1.0f - planar_blend) * global_window_parameters.fog.atmospheric_color.n[0]
                    + planar_blend * global_window_parameters.fog.planar_color.n[0]) * atmospheric_density
                    - global_window_parameters.fog.planar_color.n[0]);
            fog_delta_g = -(((1.0f - planar_blend) * global_window_parameters.fog.atmospheric_color.n[1]
                    + planar_blend * global_window_parameters.fog.planar_color.n[1]) * atmospheric_density
                    - global_window_parameters.fog.planar_color.n[1]);
            fog_delta_b = -(((1.0f - planar_blend) * global_window_parameters.fog.atmospheric_color.n[2]
                    + global_window_parameters.fog.planar_color.n[2] * planar_blend) * atmospheric_density
                    - global_window_parameters.fog.planar_color.n[2]);

            fog_clamped_r = -fog_delta_r;
            if ( fog_clamped_r < 0.0f )
                fog_clamped_r = 0.0f;
            else if ( fog_clamped_r > 1.0f )
                fog_clamped_r = 1.0f;

            fog_clamped_g = -fog_delta_g;
            if ( fog_clamped_g < 0.0f )
                fog_clamped_g = 0.0f;
            else if ( fog_clamped_g > 1.0f )
                fog_clamped_g = 1.0f;

            fog_clamped_b = -fog_delta_b;
            if ( fog_clamped_b < 0.0f )
                fog_clamped_b = 0.0f;
            else if ( fog_clamped_b > 1.0f )
                fog_clamped_b = 1.0f;

            if ( fog_delta_r < 0.0f )
                fog_delta_r = 0.0f;
            else if ( fog_delta_r > 1.0f )
                fog_delta_r = 1.0f;
            if ( fog_delta_g < 0.0f )
                fog_delta_g = 0.0f;
            else if ( fog_delta_g > 1.0f )
                fog_delta_g = 1.0f;

            if ( fog_delta_b >= 0.0f )
            {
                if ( fog_delta_b > 1.0f )
                    fog_delta_b = 1.0f;
            }
            else
            {
                fog_delta_b = 0.0f;
            }
            fog_atmos_r = atmospheric_density * global_window_parameters.fog.atmospheric_color.n[0];
            fog_atmos_g = atmospheric_density * global_window_parameters.fog.atmospheric_color.n[1];
            fog_atmos_b = global_window_parameters.fog.atmospheric_color.n[2] * atmospheric_density;
            fog_computed = 1;
        }
    }
    if ( !fog_computed )
    {
        fog_atmos_r = 0.0f;
        fog_clamped_r = 0.0f;
        fog_delta_r = 0.0f;
        fog_atmos_g = 0.0f;
        fog_clamped_g = 0.0f;
        fog_delta_g = 0.0f;
        fog_atmos_b = 0.0f;
        fog_clamped_b = 0.0f;
        fog_delta_b = 0.0f;
        fog_alpha = 1.0f;
    }

    if ( effect_shader->constants )
    {
        ID3DXEffect *effect = effect_shader->effect;
        D3DXVECTOR4 constant_vector;

        constant_vector.x = change_color_r;
        constant_vector.y = change_color_g;
        constant_vector.z = change_color_b;
        constant_vector.w = force_alpha_blend ? force_alpha : 1.0f;
        ID3DXEffect_SetVector(effect, effect_shader->constants[0], &constant_vector);

        constant_vector.x = fog_delta_r;
        constant_vector.y = fog_delta_g;
        constant_vector.z = fog_delta_b;
        constant_vector.w = fog_alpha;
        ID3DXEffect_SetVector(effect, effect_shader->constants[1], &constant_vector);

        constant_vector.x = fog_clamped_r;
        constant_vector.y = fog_clamped_g;
        constant_vector.z = fog_clamped_b;
        constant_vector.w = 1.0f;
        ID3DXEffect_SetVector(effect, effect_shader->constants[2], &constant_vector);

        constant_vector.x = fog_atmos_r;
        constant_vector.y = fog_atmos_g;
        constant_vector.z = fog_atmos_b;
        constant_vector.w = 1.0f;
        ID3DXEffect_SetVector(effect, effect_shader->constants[3], &constant_vector);
    }

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_model));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(vertex_shader_index));

    /* reflection-tint block: perpendicular/parallel view colours scaled by the per-draw reflection tint;
     * only the red channel is faded by reflection_fraction (c13 = perp-parallel deltas, c14 = parallel terms) */
    float reflection_tint_g = local_parameters->lighting.reflection_tint_color.n[1];
    float reflection_tint_b = local_parameters->lighting.reflection_tint_color.n[2];
    float reflection_tint_a = local_parameters->lighting.reflection_tint_color.n[3];
    float parallel_r = shader->model.reflection_view_parallel_color.n[0]
            * local_parameters->lighting.reflection_tint_color.n[0];
    float perp_r = shader->model.reflection_view_perpendicular_color.n[0]
            * local_parameters->lighting.reflection_tint_color.n[0];
    float perp_g = shader->model.reflection_view_perpendicular_color.n[1]
            * local_parameters->lighting.reflection_tint_color.n[1];
    float perp_b = shader->model.reflection_view_perpendicular_color.n[2] * reflection_tint_b;
    float parallel_g = shader->model.reflection_view_parallel_color.n[1] * reflection_tint_g;
    float perp_a = shader->model.reflection_view_perpendicular_color.n[3] * reflection_tint_a;
    float parallel_a = shader->model.reflection_view_parallel_color.n[3] * reflection_tint_a;
    float parallel_b = shader->model.reflection_view_parallel_color.n[2] * reflection_tint_b;
    float parallel_r_faded = parallel_r * reflection_fraction;

    /* c10..c12: detail-map UV transform (row 0) followed by the animated base-map transform (rows 1,2). The
     * presets to [4..11] are the identity fallback overwritten by shader_texture_animation_evaluate. */
    float model_vertex_constants[12];
    model_vertex_constants[0] = shader->model.detail_map_scale;
    model_vertex_constants[1] = shader->model.detail_map_scale * shader->model.detail_map_v_scale;
    model_vertex_constants[2] = 1.0f;
    model_vertex_constants[3] = 1.0f;
    model_vertex_constants[4] = 1.0f;
    model_vertex_constants[5] = 0.0f;
    model_vertex_constants[6] = 0.0f;
    model_vertex_constants[7] = 0.0f;
    model_vertex_constants[8] = 0.0f;
    model_vertex_constants[9] = 1.0f;
    model_vertex_constants[10] = 0.0f;
    model_vertex_constants[11] = 0.0f;

    /* c13/c14: reflection-tint delta block */
    float reflection_block[8];
    reflection_block[0] = perp_g - parallel_g;
    reflection_block[1] = perp_b - parallel_b;
    reflection_block[2] = perp_a - parallel_a;
    reflection_block[3] = perp_r * reflection_fraction - parallel_r_faded;
    reflection_block[4] = parallel_g;
    reflection_block[5] = parallel_b;
    reflection_block[6] = parallel_a;
    reflection_block[7] = parallel_r_faded;

    shader_texture_animation_evaluate(
            &shader->model.animation,
            &local_parameters->animation,
            local_parameters->base_map_scale.n[0] * shader->model.map_scale.n[0],
            local_parameters->base_map_scale.n[1] * shader->model.map_scale.n[1],
            0.0f, 0.0f, 0.0f,
            (float)*(double *)&global_frame_parameters.game_time_sec,
            (real_vector4d *)&model_vertex_constants[4],
            (real_vector4d *)&model_vertex_constants[8]);
    model_vertex_constants[10] = shader->model.translucency;

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, model_vertex_constants, 3,
            (unsigned __int64)3 << 60);
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, reflection_block, 2, (unsigned __int64)1 << 60);

    if ( global_rasterizer_model_ambient_reflection_tint
            && (global_rasterizer_model_ambient_reflection_tint->n[0] > 0.0f
                || global_rasterizer_model_ambient_reflection_tint->n[1] > 0.0f
                || global_rasterizer_model_ambient_reflection_tint->n[2] > 0.0f
                || global_rasterizer_model_ambient_reflection_tint->n[3] > 0.0f) )
    {
        float ambient_reflection_block[8];
        ambient_reflection_block[0] = 0.0f;
        ambient_reflection_block[1] = 0.0f;
        ambient_reflection_block[2] = 0.0f;
        ambient_reflection_block[3] = 0.0f;
        ambient_reflection_block[4] = global_rasterizer_model_ambient_reflection_tint->n[0];
        ambient_reflection_block[5] = global_rasterizer_model_ambient_reflection_tint->n[1];
        ambient_reflection_block[6] = global_rasterizer_model_ambient_reflection_tint->n[2];
        ambient_reflection_block[7] = global_rasterizer_model_ambient_reflection_tint->n[3];
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, ambient_reflection_block, 2,
                (unsigned __int64)1 << 60);
    }

    unsigned int pass_count[4];
    ID3DXEffect_Begin(effect_shader->effect, pass_count, 3);
    for ( unsigned int i = 0; i < pass_count[0]; ++i )
    {
        ID3DXEffect_BeginPass(effect_shader->effect, i);
        rasterizer_draw(triangle_buffer, dynamic_triangle_buffer_index, 0, triangle_count, vertex_buffer,
                dynamic_vertex_buffer_index);
        ID3DXEffect_EndPass(effect_shader->effect);
    }
    ID3DXEffect_End(effect_shader->effect);

    /* two-sided shader that was culled on the first pass: run a back-face pass with the detail-map transform */
    if ( (shader->model.flags & (1u << _shader_model_two_sided_bit)) && cull_enabled_first_pass )
    {
        float detail_map_scale = shader->model.detail_map_scale;
        float back_face_constants[12];
        back_face_constants[0] = detail_map_scale;
        back_face_constants[1] = shader->model.detail_map_scale * shader->model.detail_map_v_scale;
        back_face_constants[2] = 1.0f;
        back_face_constants[3] = -1.0f;
        back_face_constants[4] = 1.0f;
        back_face_constants[5] = 0.0f;
        back_face_constants[6] = 0.0f;
        back_face_constants[7] = 0.0f;
        back_face_constants[8] = 0.0f;
        back_face_constants[9] = 1.0f;
        back_face_constants[10] = 0.0f;
        back_face_constants[11] = 0.0f;

        shader_texture_animation_evaluate(
                &shader->model.animation,
                &local_parameters->animation,
                local_parameters->base_map_scale.n[0] * shader->model.map_scale.n[0],
                local_parameters->base_map_scale.n[1] * shader->model.map_scale.n[1],
                0.0f, 0.0f, 0.0f,
                (float)*(double *)&global_frame_parameters.game_time_sec,
                (real_vector4d *)&back_face_constants[4],
                (real_vector4d *)&back_face_constants[8]);
        back_face_constants[10] = shader->model.translucency;

        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, back_face_constants, 3,
                (unsigned __int64)3 << 60);

        ID3DXEffect_Begin(effect_shader->effect, pass_count, 3);
        for ( unsigned int j = 0; j < pass_count[0]; ++j )
        {
            ID3DXEffect_BeginPass(effect_shader->effect, j);
            D3DDevice_SetRenderState_CullMode(global_d3d_device, 2);
            rasterizer_draw(triangle_buffer, dynamic_triangle_buffer_index, 0, triangle_count, vertex_buffer,
                    dynamic_vertex_buffer_index);
            ID3DXEffect_EndPass(effect_shader->effect);
        }
        ID3DXEffect_End(effect_shader->effect);
    }

    rasterizer_dx9_reset_zbias();
}
