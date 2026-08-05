/* _rasterizer_environment_lightmap_draw_pp @0x8378F010 — pixel-processor lightmap draw pass for environment
 * geometry. Binds the base map (stage 0, alpha-tested only when the shader's flags bit 0 is set and alpha
 * testing is globally enabled) and a second detail/bump map (stage 1) through the effect, forces wrap or
 * clamp addressing on stage 1 depending on a shader flag packed into the spare tint_color.__s1.green slot,
 * binds the current lightmap (or a default white texture when none is set) at stage 2 and the vector-
 * normalization cube map at stage 3, uploads a texture-transform constant block (c10..c12) whose u/v scroll
 * offset is filled in by shader_environment_texture_animation_evaluate, evaluates three periodic animation
 * functions (color blend factors) and uploads the resulting six lightmap-tint vectors as effect parameters
 * (only when the shader is not the alternate/reduced variant selected by rasterizer_lightmap_no_lightmap_variant), then draws the
 * group's dynamic triangles once per effect pass. Runs only when rasterizer_debug_options.draw_environment_
 * lightmaps is set.
 *
 * DEVIATIONS (disasm-resolved):
 *  - shader_environment_texture_animation_evaluate's u_offset/v_offset out-parameters (decompiler local `v25`,
 *    never assigned in the decompiler's own output — a lost address-of) are `&texture_transform_constants[7]`
 *    and `&texture_transform_constants[11]`, the same combined-buffer convention as the sibling
 *    _rasterizer_environment_reflection_draw.c / _rasterizer_environment_reflection_lightmap_mask_draw.c
 *    (disasm 0x8378F24C/0x8378F248 confirm r5/r6 relative to the buffer base loaded at 0x8378F2A0). The
 *    decompiler's separate v54[7]/v55[5] locals are the same contiguous 12-float stack buffer split in two.
 *  - `v13`'s leading-zero-count expression (`(_cntlzw((unsigned __int8)rasterizer_lightmap_no_lightmap_variant) & 0x20) == 0) + 2`) is
 *    the decompiler's rendering of the plain `rasterizer_lightmap_no_lightmap_variant ? 3 : 2` (cntlz of a zero byte is 32, of a
 *    nonzero byte is <32 — the `&0x20` test is just an elaborate nonzero check).
 *  - Both HIWORD reads (the stage-1 addressing-mode flag and the second periodic_function_evaluate's
 *    function-type index) are plain 16-bit loads at the field's own address — disasm `lhz r11, 0x180(r31)` /
 *    `lhz r3, 0x1F0(r31)` = self_illumination.flags / self_illumination.secondary_animation_function.
 *  - shader pointer retyped from the decompiler's shader[N] 40-byte indexing to shader_environment* with
 *    named members (same convention as the sibling _rasterizer_environment_*_draw files); every offset
 *    verified against the DB shader_environment layout. */

#include <stdint.h>
#include "headers/shader_environment.h"
#include "headers/vertex_buffer.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_globals.h"
#include "headers/global_frame_parameters.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"
#include "headers/shader_environment_flags.h"
#include "headers/shader_environment_self_illumination_map_point_sampled_flags.h"

#include "headers/bitmap_data.h"
#include "headers/point2d.h"
extern uint8_t rasterizer_lightmap_no_lightmap_variant;

extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, float time_value, float *u_offset, float *v_offset);
extern float periodic_function_evaluate(int16_t function_type, float time);
extern void rasterizer_draw_dynamic_triangles_static_vertices2(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer0, const vertex_buffer *vertex_buffer1);

void __fastcall rasterizer_environment_lightmap_draw_pp(const shader *shader, int16_t shader_permutation_index,
        int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count,
        const vertex_buffer *vertex_buffer)
{
    const shader_environment *shader_env = (const shader_environment *)shader;

    if ( !rasterizer_debug_options.draw_environment_lightmaps )
        return;

    unsigned int alpha_test_enable = 0;
    if ( (shader_env->environment.flags & (1u << _shader_environment_alpha_tested_bit)) != 0 && rasterizer_debug_options.environment_alpha_testing_enabled )
        alpha_test_enable = 1;
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, alpha_test_enable);

    /* Selects one of the four environment-lightmap effect variants (index 0..3). When there is no second
     * (detail/illumination) map (color.n[0] == -1) the no-illumination variants are used; otherwise the
     * lit variants. rasterizer_lightmap_no_lightmap_variant chooses the no-lightmap sub-variant. */
    int16_t shader_variant;
    if ( shader_env->environment.self_illumination.map.index == -1 )
        shader_variant = rasterizer_lightmap_no_lightmap_variant ? _dxshader_environment_lightmap_no_illumination_no_lightmap
                                       : _dxshader_environment_lightmap_no_illumination;
    else
        shader_variant = rasterizer_lightmap_no_lightmap_variant ? _dxshader_environment_lightmap_no_lightmap
                                       : _dxshader_environment_lightmap_normal;

    rasterizer_dx9_shader *dxeffect_shader = rasterizer_shader_select(shader_variant);
    if ( !dxeffect_shader || !dxeffect_shader->effect )
        return;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment_lightmap));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_environment_lightmap));

    int base_map_bitmap_group_index = -1;
    if ( (shader_env->environment.flags & (1u << _shader_environment_bump_map_is_specular_mask_bit)) == 0 )
        base_map_bitmap_group_index = shader_env->environment.diffuse.bump_map.index;
    rasterizer_set_texture_for_effect(0, 0, 3, base_map_bitmap_group_index, shader_permutation_index,
            dxeffect_shader);
    rasterizer_set_texture_for_effect(1, 0, 0, shader_env->environment.self_illumination.map.index,
            shader_permutation_index, dxeffect_shader);

    if ( (shader_env->environment.self_illumination.flags & (1u << _shader_environment_self_illumination_map_point_sampled_bit)) != 0 )
    {
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 0);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 0);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 0);
    }
    else
    {
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 1);
    }

    if ( global_lightmap )
        rasterizer_set_texture_bitmap_data_for_effect(2, global_lightmap, dxeffect_shader);
    else
        dxeffect_shader->effect->lpVtbl->SetTexture(dxeffect_shader->effect, dxeffect_shader->texture[2],
                (D3DBaseTexture *)rasterizer_globals.default_white_hardware_format);
    rasterizer_set_texture_direct_for_effect(3, global_rasterizer_data->vector_normalization.index, 0,
            dxeffect_shader);

    /* c10..c12: base-map UV scale (shader-derived) then identity rows; [7]/[11] filled in below */
    float texture_transform_constants[12];
    texture_transform_constants[0]  = shader_env->environment.diffuse.runtime_bump_map_scale.__s1.i;
    texture_transform_constants[1]  = shader_env->environment.diffuse.runtime_bump_map_scale.__s1.j;
    texture_transform_constants[2]  = shader_env->environment.self_illumination.map_scale;
    texture_transform_constants[3]  = 1.0f;
    texture_transform_constants[4]  = 1.0f;
    texture_transform_constants[5]  = 0.0f;
    texture_transform_constants[6]  = 0.0f;
    texture_transform_constants[7]  = 0.0f;
    texture_transform_constants[8]  = 0.0f;
    texture_transform_constants[9]  = 1.0f;
    texture_transform_constants[10] = 0.0f;
    texture_transform_constants[11] = 0.0f;
    shader_environment_texture_animation_evaluate(shader, global_frame_parameters.game_time_sec,
            &texture_transform_constants[7], &texture_transform_constants[11]);
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, texture_transform_constants, 3,
            (uint64_t)3 << 60);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment_lightmap));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_environment_lightmap));

    float blend_1 = periodic_function_evaluate(shader_env->environment.self_illumination.primary_animation_function,
            (shader_env->environment.self_illumination.primary_animation_phase + global_frame_parameters.game_time_sec)
                    / shader_env->environment.self_illumination.primary_animation_period);
    float blend_2 = periodic_function_evaluate(shader_env->environment.self_illumination.secondary_animation_function,
            (shader_env->environment.self_illumination.secondary_animation_phase + global_frame_parameters.game_time_sec)
                    / shader_env->environment.self_illumination.secondary_animation_period);
    float blend_3 = periodic_function_evaluate(shader_env->environment.self_illumination.plasma_animation_function,
            (shader_env->environment.self_illumination.plasma_animation_phase + global_frame_parameters.game_time_sec)
                    / shader_env->environment.self_illumination.plasma_animation_period);

    unsigned int *constants = dxeffect_shader->constants;

    float lightmap_tint_0[4];
    lightmap_tint_0[0] = shader_env->environment.diffuse.material_color.red;
    lightmap_tint_0[1] = shader_env->environment.diffuse.material_color.green;
    lightmap_tint_0[2] = shader_env->environment.diffuse.material_color.blue;
    lightmap_tint_0[3] = 1.0f;
    if ( constants )
        dxeffect_shader->effect->lpVtbl->SetVector(dxeffect_shader->effect, *constants, (const D3DXVECTOR4 *)lightmap_tint_0);

    if ( !shader_variant )
    {
        constants = dxeffect_shader->constants;

        float lightmap_tint_1[4];
        lightmap_tint_1[0] = blend_3;
        lightmap_tint_1[1] = blend_3;
        lightmap_tint_1[2] = blend_3;
        lightmap_tint_1[3] = 0.5f - blend_3;

        float lightmap_tint_2[4];
        lightmap_tint_2[0] = shader_env->environment.self_illumination.primary_on_color.red * blend_1
                + shader_env->environment.self_illumination.primary_off_color.red * (1.0f - blend_1);
        lightmap_tint_2[1] = shader_env->environment.self_illumination.primary_on_color.green * blend_1
                + shader_env->environment.self_illumination.primary_off_color.green * (1.0f - blend_1);
        lightmap_tint_2[2] = shader_env->environment.self_illumination.primary_on_color.blue * blend_1
                + shader_env->environment.self_illumination.primary_off_color.blue * (1.0f - blend_1);
        lightmap_tint_2[3] = 1.0f;

        float lightmap_tint_3[4];
        lightmap_tint_3[0] = shader_env->environment.self_illumination.secondary_on_color.red * blend_2
                + shader_env->environment.self_illumination.secondary_off_color.red * (1.0f - blend_2);
        lightmap_tint_3[1] = shader_env->environment.self_illumination.secondary_on_color.green * blend_2
                + shader_env->environment.self_illumination.secondary_off_color.green * (1.0f - blend_2);
        lightmap_tint_3[2] = shader_env->environment.self_illumination.secondary_on_color.blue * blend_2
                + shader_env->environment.self_illumination.secondary_off_color.blue * (1.0f - blend_2);
        lightmap_tint_3[3] = 1.0f;

        float lightmap_tint_4[4];
        lightmap_tint_4[0] = shader_env->environment.self_illumination.plasma_on_color.red;
        lightmap_tint_4[1] = shader_env->environment.self_illumination.plasma_on_color.green;
        lightmap_tint_4[2] = shader_env->environment.self_illumination.plasma_on_color.blue;
        lightmap_tint_4[3] = 1.0f;

        float lightmap_tint_5[4];
        lightmap_tint_5[0] = shader_env->environment.self_illumination.plasma_off_color.red;
        lightmap_tint_5[1] = shader_env->environment.self_illumination.plasma_off_color.green;
        lightmap_tint_5[2] = shader_env->environment.self_illumination.plasma_off_color.blue;
        lightmap_tint_5[3] = 1.0f;

        if ( constants )
        {
            dxeffect_shader->effect->lpVtbl->SetVector(dxeffect_shader->effect, constants[1], (const D3DXVECTOR4 *)lightmap_tint_1);
            dxeffect_shader->effect->lpVtbl->SetVector(dxeffect_shader->effect, constants[2], (const D3DXVECTOR4 *)lightmap_tint_2);
            dxeffect_shader->effect->lpVtbl->SetVector(dxeffect_shader->effect, constants[3], (const D3DXVECTOR4 *)lightmap_tint_3);
            dxeffect_shader->effect->lpVtbl->SetVector(dxeffect_shader->effect, constants[4], (const D3DXVECTOR4 *)lightmap_tint_4);
            dxeffect_shader->effect->lpVtbl->SetVector(dxeffect_shader->effect, constants[5], (const D3DXVECTOR4 *)lightmap_tint_5);
        }
    }

    unsigned int pass_count[4];
    dxeffect_shader->effect->lpVtbl->Begin(dxeffect_shader->effect, pass_count, 3);
    for ( unsigned int pass = 0; pass < pass_count[0]; ++pass )
    {
        dxeffect_shader->effect->lpVtbl->BeginPass(dxeffect_shader->effect, pass);
        rasterizer_draw_dynamic_triangles_static_vertices2(dynamic_triangle_buffer_index, first_triangle_index,
                triangle_count, vertex_buffer, &vertex_buffer[rasterizer_lightmap_no_lightmap_variant == 0]);
        dxeffect_shader->effect->lpVtbl->EndPass(dxeffect_shader->effect);
    }
    dxeffect_shader->effect->lpVtbl->End(dxeffect_shader->effect);
}
