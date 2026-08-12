/* rasterizer_model_draw_environment_shader_pp @0x8378B800 — the pixel-processor (post-2.0-shader) variant of
 * the model "environment" shader draw: sets the fixed render state for opaque/alpha-tested geometry, selects
 * the environment-mapped technique (masked or unmasked, keyed off a per-shader technique-variant field),
 * binds base/bump/specular/reflection maps, uploads the base-map UV scale and a reflection-tint delta block,
 * computes the per-draw planar/atmospheric fog blend (or copies the reflection-tint terms through unchanged
 * when the shader's no-fog flag is set), pushes the fog constants to the effect, then draws the dynamic
 * triangles once per effect pass.
 *
 * Clean decompile. The decompiler's 40-byte shader[N] type-puns are resolved to the shader_environment DB
 * members (environment.flags, diffuse.base/detail/bump maps, specular perpendicular/parallel colours,
 * reflection brightnesses + map). DEVIATION (disasm 0x8378BB58-0x8378BB78, 0x8378BB74-0x8378BB8C): the decompiler garbled both
 * D3DDevice_SetVertexShaderConstantFN calls' trailing args (FPR/GPR-shadow pun) — count/mask are actually
 * (device, 0xA, base_map_transform, 3, (uint64)3<<60) and (device, 0xD, reflection_block, 2, (uint64)1<<60),
 * confirmed against the raw immediates (li r6,3/sldi r7,r7,60 and li r6,2/li r7,1;extldi r7,r7,64,60 — the
 * latter is rotr64(1,4), i.e. 1<<60 — not the decompiler's HIDWORD-only __ROR4__(1,4) reading). */

#include <stdint.h>
#include "headers/fog_definition_flags.h"
#include "headers/shader.h"
#include "headers/shader_environment.h"
#include "headers/shader_environment_flags.h"
#include "headers/vertex_buffer.h"
#include "headers/triangle_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_model_techniques.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"


#include "headers/point2d.h"
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
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);

extern void rasterizer_dx9_reset_zbias(void);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern long rasterizer_set_technique(ID3DXEffect *effect, unsigned int technique);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_draw(const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);

void rasterizer_model_draw_environment_shader_pp(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index)
{
    const shader_environment *env = (const shader_environment *)shader;
    if ( !rasterizer_debug_options.draw_models )
        return;

    float camera_relative_x = local_parameters->centroid.n[0] - global_window_parameters.camera.position.n[0];
    float camera_relative_y = local_parameters->centroid.n[1] - global_window_parameters.camera.position.n[1];
    float camera_relative_z = local_parameters->centroid.n[2] - global_window_parameters.camera.position.n[2];

    if ( local_parameters->geometry_flags & (1u << _rasterizer_geometry_no_zbuffer_bit) )
    {
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);            /* D3DZB_FALSE */
    }
    else
    {
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);            /* D3DZB_TRUE */
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 1);       /* TRUE */
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_LESSEQUAL);              /* D3DCMP_LESSEQUAL */
        rasterizer_dx9_reset_zbias();
    }
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);               /* D3DCULL_CCW */
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);       /* RED|GREEN|BLUE color-write mask */
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);       /* FALSE */
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_SRCALPHA);               /* D3DBLEND_SRCALPHA */
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVSRCALPHA);              /* D3DBLEND_INVSRCALPHA */
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);                /* D3DBLENDOP_ADD */
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device,
            env->environment.flags & (1u << _shader_environment_alpha_tested_bit));
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7F);            /* alpha test reference 127 */

    int16_t vertex_shader_index = 28;
    if ( (env->environment.flags & (1u << _shader_environment_true_atmospheric_fog_bit)) == 0 )
    {
        if ( local_planar_fog_flag )
            vertex_shader_index = 25;
        else if ( local_parameters->lighting.point_light_count > 0 )
            vertex_shader_index = 26;
        else if ( env->environment.reflection.map.index == -1
                && local_parameters->skinning.node_matrix_count <= 1 )
            vertex_shader_index = 29;
    }

    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_model_environment);
    if ( !effect_shader || !effect_shader->effect )
    {
        rasterizer_dx9_reset_zbias();
        return;
    }

    int16_t technique_variant = env->environment.diffuse.detail_map_function;
    unsigned int technique = (env->environment.flags & (1u << _shader_environment_true_atmospheric_fog_bit)) != 0
            ? hModelEnvironmentNoMaskTechniques[technique_variant]
            : hModelEnvironmentNoMaskTechniques[technique_variant + 6];
    rasterizer_set_technique(effect_shader->effect, technique);

    rasterizer_set_texture_for_effect(0, 0, 1, env->environment.diffuse.base_map.index,
            shader_permutation_index, effect_shader);
    rasterizer_set_texture_for_effect(1, 0, 2, env->environment.diffuse.primary_detail_map.index,
            shader_permutation_index, effect_shader);

    /* stage 2: the bump map doubles as the specular mask, bound only for alpha-tested shaders */
    int bump_map_index = (env->environment.flags & (1u << _shader_environment_alpha_tested_bit)) != 0
            ? env->environment.diffuse.bump_map.index
            : -1;
    rasterizer_set_texture_for_effect(2, 0, 1, bump_map_index, shader_permutation_index, effect_shader);

    rasterizer_set_texture_for_effect(3, 2, 0, env->environment.reflection.map.index,
            shader_permutation_index, effect_shader);

    /* reflection-tint terms: the specular view-perpendicular/parallel colours and the reflection
     * view brightnesses, each scaled by local_parameters's per-draw reflection tint color */
    float reflection_r = local_parameters->lighting.reflection_tint_color.n[0];
    float reflection_g = local_parameters->lighting.reflection_tint_color.n[1];
    float reflection_b = local_parameters->lighting.reflection_tint_color.n[2];
    float reflection_a = local_parameters->lighting.reflection_tint_color.n[3];

    float perp_brightness_term = env->environment.reflection.view_perpendicular_brightness * reflection_r;
    float perp_red_term = env->environment.specular.view_perpendicular_color.n[0] * reflection_g;
    float perp_green_term = env->environment.specular.view_perpendicular_color.n[1] * reflection_b;
    float par_brightness_term = env->environment.reflection.view_parallel_brightness * reflection_r;
    float par_red_term = env->environment.specular.view_parallel_color.n[0] * reflection_g;
    float par_green_term = env->environment.specular.view_parallel_color.n[1] * reflection_b;
    float par_blue_term = env->environment.specular.view_parallel_color.n[2] * reflection_a;
    float perp_blue_term = env->environment.specular.view_perpendicular_color.n[2] * reflection_a;

    /* c10..c12: primary-detail-map UV scale followed by a fixed 1,1 pair */
    float base_map_transform[12];
    base_map_transform[0] = env->environment.diffuse.primary_detail_map_scale;
    base_map_transform[1] = base_map_transform[0];
    base_map_transform[2] = 1.0f;
    base_map_transform[3] = 1.0f;
    base_map_transform[4] = local_parameters->base_map_scale.n[0];
    base_map_transform[5] = 0.0f;
    base_map_transform[6] = 0.0f;
    base_map_transform[7] = 0.0f;
    base_map_transform[8] = 0.0f;
    base_map_transform[9] = local_parameters->base_map_scale.n[1];
    base_map_transform[10] = 0.0f;
    base_map_transform[11] = 0.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, base_map_transform, 3,
            (uint64_t)3 << 60);

    /* c13/c14: reflection-tint delta block (diffs) followed by the raw per-channel scaled terms */
    float reflection_block[8];
    reflection_block[0] = perp_red_term - par_red_term;
    reflection_block[1] = perp_green_term - par_green_term;
    reflection_block[2] = perp_blue_term - par_blue_term;
    reflection_block[3] = perp_brightness_term - par_brightness_term;
    reflection_block[4] = par_red_term;
    reflection_block[5] = par_green_term;
    reflection_block[6] = par_blue_term;
    reflection_block[7] = par_brightness_term;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, reflection_block, 2,
            (uint64_t)1 << 60);

    float fog_alpha;
    float fog_delta_r, fog_delta_g, fog_delta_b;
    float fog_clamped_r, fog_clamped_g, fog_clamped_b;
    float fog_atmos_r, fog_atmos_g, fog_atmos_b;
    unsigned char fog_computed = 0;

    if ( global_fog_enabled )
    {
        if ( (env->environment.flags & (1u << _shader_environment_true_atmospheric_fog_bit)) != 0 )
        {
            /* no-fog shader variant: pass the reflection-tint terms straight through */
            fog_alpha = reflection_block[0];
            fog_delta_r = reflection_block[1];
            fog_delta_g = reflection_block[2];
            fog_delta_b = reflection_block[3];
            fog_clamped_r = reflection_block[1];
            fog_clamped_g = reflection_block[2];
            fog_clamped_b = reflection_block[3];
            fog_atmos_r = reflection_block[2];
            fog_atmos_g = reflection_block[1];
            fog_atmos_b = reflection_block[0];
            fog_computed = 1;
        }
        else if ( (local_parameters->geometry_flags & (1u << _rasterizer_geometry_no_fog_bit)) == 0 )
        {
            float plane_distance = (global_window_parameters.fog.plane.n.n[0] * global_window_parameters.camera.position.n[0]
                    + (global_window_parameters.fog.plane.n.n[2] * global_window_parameters.camera.position.n[2]
                        + global_window_parameters.fog.plane.n.n[1] * global_window_parameters.camera.position.n[1]))
                    - global_window_parameters.fog.plane.d;
            float planar_blend = plane_distance / global_window_parameters.fog.atmospheric_maximum_distance;
            if ( planar_blend < 0.0f )
                planar_blend = 0.0f;
            else if ( planar_blend > 1.0f )
                planar_blend = 1.0f;

            float view_depth = (global_window_parameters.camera.forward.n[1] * camera_relative_y
                    + (camera_relative_x * global_window_parameters.camera.forward.n[0]
                        + global_window_parameters.camera.forward.n[2] * camera_relative_z));
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
                    + planar_blend * global_window_parameters.fog.planar_color.n[2]) * atmospheric_density
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

            /* DEVIATION-preserving quirk: the third channel's lower clamp bound is 0 only in the else
             * branch below; the upper bound is only enforced when -fog_delta_b >= 0 (shipped asymmetry) */
            fog_clamped_b = -fog_delta_b;
            if ( -fog_delta_b >= 0.0f )
            {
                if ( fog_clamped_b > 1.0f )
                    fog_clamped_b = 1.0f;
            }
            else
            {
                fog_clamped_b = 0.0f;
            }

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
                fog_atmos_r = atmospheric_density * global_window_parameters.fog.atmospheric_color.n[0];
                fog_atmos_g = atmospheric_density * global_window_parameters.fog.atmospheric_color.n[1];
                fog_atmos_b = global_window_parameters.fog.atmospheric_color.n[2] * atmospheric_density;
            }
            else
            {
                fog_delta_b = 0.0f;
                fog_atmos_r = atmospheric_density * global_window_parameters.fog.atmospheric_color.n[0];
                fog_atmos_g = atmospheric_density * global_window_parameters.fog.atmospheric_color.n[1];
                fog_atmos_b = global_window_parameters.fog.atmospheric_color.n[2] * atmospheric_density;
            }
            fog_computed = 1;
        }
    }
    if ( !fog_computed )
    {
        /* shared fallthrough: reached both when global fog is disabled and when it's enabled but the
         * geometry has its own per-triangle fog (geometry_flags & (1u << _rasterizer_geometry_no_fog_bit)) with no shader-level no-fog flag */
        fog_atmos_r = 0.0f;
        fog_clamped_r = 0.0f;
        fog_delta_r = 0.0f;
        fog_atmos_g = 0.0f;
        fog_clamped_g = 0.0f;
        fog_delta_g = 0.0f;
        /* DEVIATION @0x8378BD8C: the block zeroes ten fog locals, not nine — f31 (fog_atmos_b,
         * stored to c[3].z @0x8378BE88) already holds __real_00000000 from 0x8378BB04 and is the
         * source of the nine fmr's, so the decompiler dropped its own assignment. */
        fog_atmos_b = 0.0f;
        fog_alpha = 1.0f;
        fog_clamped_b = 0.0f;
        fog_delta_b = 0.0f;
    }

    if ( effect_shader->constants )
    {
        ID3DXEffect *effect = effect_shader->effect;
        D3DXVECTOR4 constant_vector;

        constant_vector.x = 1.0f;
        constant_vector.y = 1.0f;
        constant_vector.z = 1.0f;
        constant_vector.w = 1.0f;
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

    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(vertex_shader_index));
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_model));

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
    rasterizer_dx9_reset_zbias();
}
