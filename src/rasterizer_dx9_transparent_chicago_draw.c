/* rasterizer_dx9_transparent_chicago_draw @0x8382E838 — draws one presorted transparent-geometry group whose
 * shader is a shader_transparent_chicago tag. It: (1) binds the vertex shader/declaration for the group's
 * primary vertex type + permutation and clears the pixel shader; (2) recursively draws each of the shader's
 * "extra layers" (sub-shaders) as their own groups; (3) sets the fixed transparent render state (cull, blend,
 * alpha test, framebuffer blend function); (4) optionally advances the animated frame index; (5) for each of
 * the up to 4 texture stages, binds the stage bitmap and its sampler address/filter state and builds an
 * 8-float-per-stage texture-transform vertex constant (either a texture-animation evaluation or, for
 * environment-mapped stages, the view-space X/Y basis vectors); (6) uploads both vertex-constant buffers,
 * builds the chicago pixel-shader config, draws, and restores the blend op.
 *
 * Reconstructed from disassembly (0x8382E85C-0x8382F018); the decompiler printed "local variable allocation
 * has failed". Deviations, all verified per-instruction:
 *   - `v30 = __PAIR64__(bitmap_address_table, __ROR4__(1,1))` is a DECOMPILER FABRICATION. Disasm at
 *     0x8382EB74/EB88 shows `li r9,1; extldi r24,r9,64,63` producing the standalone 64-bit constant
 *     1<<63 in r24; the adjacent `lis r11, bitmap_address_table@ha` (the legitimate array-base load for the
 *     bitmap_address_table[detail_level]/bitmap_type_table[detail_level] indexing) was fused into a bogus
 *     __PAIR64__. The per-stage value `v37 = v30 >> (stage+32)` is really `(1<<63) >> (stage+32)` =
 *     `1<<(31-stage)`, the sampler dirty bit OR'd into the device's pending mask — that bookkeeping is
 *     internal to the D3DDevice_SetSamplerState_*_Inline accessors, so it is expressed through those calls
 *     rather than modeled. bitmap_address_table (int[4]={0,2,2,2} @0x8212A064) and bitmap_type_table
 *     (u16[4]={0,2,2,2} @0x8212A05C) are genuine detail-level lookup tables.
 *   - Both D3DDevice_SetVertexShaderConstantFN calls: real (StartRegister, ..., Vector4fCount, PendingMask0)
 *     args are (0xD, texture_transform_constants, 8, 7<<58) and (0xA, color_constants, 3, 3<<60) — disasm
 *     0x8382EF14/EFDC. The decompiler's 4th/5th positional args are FPR/GPR-shadow garble.
 *   - The AddressU/V/W, MagFilter, MinFilter and SeparateZFilterEnable pokes read as raw m_Pending.m_Mask /
 *     m_Constants.Fetch writes are the compiler-inlined named sampler accessors (D3DDevice is a boundary type
 *     per d3d_boundary.h; same convention as rasterizer_plasma_energy_draw / rasterizer_water_build_bumpmap).
 *     SeparateZFilterEnable's inline form also clears an adjacent border-color bit (rlwinm ,8,6 at 0x8382ED58).
 *   - The texture-transform out pointers for shader_texture_animation_evaluate are computed fresh in the
 *     texture-animation branch (var_1C0+32*stage and var_1B0+32*stage, disasm 0x8382EE18-EE30) — i.e. the
 *     k0 and k4 slots of this stage's 8-float span; the decompiler's cross-branch v15/v16 flow was an
 *     artifact. Both branches fill the same 32-float texture_transform_constants buffer.
 *   - shader-tag fields are read through the DB-typed _shader_transparent_chicago body at shader+0x28
 *     (numeric_counter_limit/flags@0x28, type@0x2A, framebuffer_blend_function@0x2C,
 *     framebuffer_fade_source@0x30, extra_layers@0x48/0x4C, maps@0x54/0x58, extra_flags@0x60) — the
 *     decompiler's shader_tag[N].base.radiosity.* word puns read these same byte offsets; per-stage
 *     stride 220 with flags@0, u/v scale@84, u/v/r offsets@92/96/100, bitmap group@120, tex-anim@164. */

#include <stdint.h>
#include <string.h>
#include "headers/shader_transparent_chicago_extra_flags.h"
#include "headers/point2d.h"
#include "headers/bitmap_group.h"
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/render_animation.h"
#include "headers/shader_texture_animation.h"
#include "headers/shader_transparent_chicago.h"
#include "headers/shader_transparent_chicago_flags.h"
#include "headers/shader_transparent_chicago_map.h"
#include "headers/shader_transparent_chicago_map_flags.h"
#include "headers/real_vector4d.h"
#include "headers/real_matrix4x3.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/global_frame_parameters.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_reference.h"
#include "headers/d3d_boundary.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/render_model_effect_type.h"


/* detail-level lookup tables (.rdata) */
#include "headers/shader_texture_animation.h"
#include "headers/point2d.h"
extern const int bitmap_address_table[4];             /* {0,2,2,2} @0x8212A064 */
extern const uint16_t bitmap_type_table[4];   /* {0,2,2,2} @0x8212A05C */
/* [6*primary_vertex_type + permutation] -> vertex-shader index (u16) @0x84186DC8 */

/* D3D device state (thin wrappers / compiler-inlined accessors over the command buffer) */
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);

/* rasterizer / shader helpers */
extern int16_t shader_get_vertex_shader_permutation(const shader *shader);
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern void rasterizer_transparent_geometry_group_draw(const transparent_geometry_group *group, uint8_t dirty);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);
extern point2d *rasterizer_set_texture(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);
extern void rasterizer_set_framebuffer_blend_function(int16_t framebuffer_blend_function);
extern void shader_texture_animation_evaluate(const shader_texture_animation *texture_animation, const render_animation *render_animation, float u_scale, float v_scale, float u_offset, float v_offset, float r_offset, float time_value, real_vector4d *u_transform_reference, real_vector4d *v_transform_reference);
extern int16_t numeric_countdown_timer_get(int16_t digit_index);
extern int shader_transparent_chicago_create(const shader *shader, int use_additional_op);
extern double floor(double x);

void rasterizer_dx9_transparent_chicago_draw(const transparent_geometry_group *group, uint8_t dirty)
{
    int16_t vertex_shader_permutation = shader_get_vertex_shader_permutation(group->shader);
    int16_t primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);
    const shader *shader_tag = group->shader;   /* renamed from 'shader' to avoid shadowing the shader type */
    /* typed view of the chicago-specific body at shader+0x28 */
    const _shader_transparent_chicago *chicago = &((const shader_transparent_chicago *)shader_tag)->chicago;
    int16_t animation_frame_index = group->shader_permutation_index;
    int use_additional_op = 0;

    /* color_constants -> vertex reg 0xA (3 vector4f); only the alpha slot [10] varies at runtime */
    float color_constants[12] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    /* texture_transform_constants -> vertex reg 0xD (8 vector4f); 8 floats per texture stage */
    float texture_transform_constants[32];

    shader_transparent_chicago_map *stage_array =
        (shader_transparent_chicago_map *)chicago->maps.address;
    if ( !stage_array || !stage_array->map.name )
        return;

    D3DVertexShader *vshader =
        rasterizer_dx9_shaders_vshader9_get(_translation_table[6 * primary_vertex_type + vertex_shader_permutation]);
    D3DDevice_SetVertexShader(global_d3d_device, vshader);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(primary_vertex_type));
    D3DDevice_SetPixelShader(global_d3d_device, nullptr);

    /* draw each extra shader layer (sub-shader) as its own transparent group */
    int extra_layer_count = chicago->extra_layers.count;
    if ( extra_layer_count > 0 )
    {
        tag_reference *extra_layer_table = (tag_reference *)chicago->extra_layers.address;
        for ( int i = 0; i < extra_layer_count; i = (int16_t)(i + 1) )
        {
            transparent_geometry_group sub_group;
            memcpy(&sub_group, group, sizeof(sub_group));
            sub_group.sorted_index = -1;
            /* extra-layer block is a tag_reference[]; .index (@+0xC) selects the sub-shader tag */
            sub_group.shader = TAG_GET(const shader, extra_layer_table[i].index);
            rasterizer_transparent_geometry_group_draw(&sub_group, dirty);
        }
    }

    D3DDevice_SetRenderState_CullMode(global_d3d_device,
        (chicago->flags & (1u << _shader_transparent_chicago_two_sided_bit)) != 0 ? 0 /* D3DCULL_NONE */ : 6 /* D3DCULL_CCW */);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, chicago->flags & (1u << _shader_transparent_chicago_alpha_tested_bit));
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7F);
    rasterizer_set_framebuffer_blend_function(chicago->framebuffer_blend_function);

    /* animated frame selection (only when the "animated" flag is set and the group has an animation) */
    if ( (chicago->flags & (1u << _shader_transparent_chicago_numeric_bit)) != 0 && group->animation && chicago->maps.count > 0 )
    {
        /* frame count of the first stage's bitmap group */
        int bitmap_frame_count = TAG_GET(bitmap_group, stage_array->map.index)->bitmaps.count;
        if ( (chicago->extra_flags & (1u << _shader_transparent_chicago_numeric_countdown_timer_bit)) != 0 )
        {
            animation_frame_index = numeric_countdown_timer_get(group->shader_permutation_index);
        }
        else
        {
            int frame_count = (int16_t)bitmap_frame_count;
            int values_index = (bitmap_frame_count != 8) ? 0 : 3;
            /* numeric_counter_limit (byte @0x28): doubles as the animation scale and the frame clamp ceiling */
            float scale = (float)chicago->numeric_counter_limit;
            int frame = (int)(float)floor((group->animation->values[values_index] * scale) + 0.5f);
            if ( frame < 0 )
            {
                frame = 0;
            }
            else if ( frame > (int)scale )   /* clamp to [0, scale]; both disasm branches recompute the same value */
            {
                frame = (int)scale;
            }
            for ( int i = 0; i < group->shader_permutation_index; i = (int16_t)(i + 1) )
                frame /= frame_count;
            animation_frame_index = frame % frame_count;
        }
    }

    for ( int stage = 0; stage < 4; stage = (int16_t)(stage + 1) )
    {
        int stage_count = chicago->maps.count;

        /* --- bind bitmap + sampler state for this real texture stage --- */
        if ( stage < stage_count )
        {
            int16_t first_map_type = chicago->type;
            shader_transparent_chicago_map *stage_ptr = &stage_array[stage];
            int16_t bitmap_type = stage ? 0 : bitmap_type_table[first_map_type];
            rasterizer_set_texture(stage, bitmap_type, 0, stage_ptr->map.index, animation_frame_index);

            unsigned int address_u = (bitmap_type || (stage_ptr->flags & (1u << _shader_transparent_chicago_map_u_clamped_bit)) == 0)
                                         ? (stage ? 0 : bitmap_address_table[first_map_type]) : 2;
            unsigned int address_v = (bitmap_type || (stage_ptr->flags & (1u << _shader_transparent_chicago_map_v_clamped_bit)) == 0)
                                         ? (stage ? 0 : bitmap_address_table[first_map_type]) : 2;
            unsigned int address_w = stage ? 0 : bitmap_address_table[first_map_type];
            unsigned int filter_flag = (stage_ptr->flags & (1u << _shader_transparent_chicago_map_point_sampled_bit)) == 0;

            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, stage, address_u);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, stage, address_v);
            D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, stage, address_w);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, stage, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, stage, filter_flag);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, stage, filter_flag);
        }

        /* --- build this stage's 8-float texture-transform constant --- */
        real_vector4d *out0 = (real_vector4d *)&texture_transform_constants[8 * stage];
        real_vector4d *out1 = (real_vector4d *)&texture_transform_constants[8 * stage + 4];

        if ( stage < stage_count && (stage > 0 || !chicago->type) )
        {
            /* texture-animation path: evaluate the stage's uv scroll/scale into out0/out1 */
            shader_transparent_chicago_map *stage_ptr = &stage_array[stage];
            /* DEVIATION: the decompiler's 64-bit punned {u,v} scale copy (ld/std @0x8382ED9C) is a plain
               real_vector2d read */
            float scale_u = stage_ptr->scale.n[0];
            float scale_v = stage_ptr->scale.n[1];
            int apply_base_map_scale = 1;
            if ( !stage && (chicago->flags & (1u << _shader_transparent_chicago_scale_first_map_with_distance_bit)) != 0 )
            {
                scale_u = -(group->z_sort * stage_ptr->scale.n[0]);
                scale_v = -(group->z_sort * stage_ptr->scale.n[1]);
            }
            if ( (chicago->flags & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0 )
                apply_base_map_scale = 0;   /* env-mapping: skip the base-map-scale multiply */
            if ( apply_base_map_scale )
            {
                scale_u = (group->model_base_map_scale.n[0] * scale_u);
                scale_v = (group->model_base_map_scale.n[1] * scale_v);
            }
            shader_texture_animation_evaluate(
                &stage_ptr->animation,
                group->animation, scale_u, scale_v,
                stage_ptr->offset.n[0], stage_ptr->offset.n[1], stage_ptr->rotation,
                (float)global_frame_parameters.game_time_sec,
                out0, out1);
        }
        else if ( stage < stage_count && (chicago->flags & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0 )
        {
            /* environment-mapping path: pack the view-space X and Y basis vectors for reflection texcoords */
            const real_matrix4x3 *view_to_world = &global_window_parameters.frustum.view_to_world;
            texture_transform_constants[8 * stage + 0] = view_to_world->n[0][0];
            texture_transform_constants[8 * stage + 1] = view_to_world->n[0][1];
            texture_transform_constants[8 * stage + 2] = view_to_world->n[0][2];
            texture_transform_constants[8 * stage + 4] = view_to_world->n[1][0];
            texture_transform_constants[8 * stage + 5] = view_to_world->n[1][1];
            texture_transform_constants[8 * stage + 6] = view_to_world->n[1][2];
        }
        else
        {
            /* default (identity-ish) transform */
            texture_transform_constants[8 * stage + 0] = 1.0f;
            texture_transform_constants[8 * stage + 1] = 0.0f;
            texture_transform_constants[8 * stage + 2] = 0.0f;
            texture_transform_constants[8 * stage + 4] = 0.0f;
            texture_transform_constants[8 * stage + 5] = 1.0f;
            texture_transform_constants[8 * stage + 6] = 0.0f;
        }
        texture_transform_constants[8 * stage + 7] = 0.0f;   /* k7 always 0 */
        texture_transform_constants[8 * stage + 3] = 0.0f;   /* k3 always 0 */
    }

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, texture_transform_constants, 8, (uint64_t)7 << 58);

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_sky_bit)) == 0 || chicago->framebuffer_blend_function )
    {
        use_additional_op = 1;
        int16_t alpha_animation_index = chicago->framebuffer_fade_source;

        if ( group->effect.type == _render_model_effect_type_active_camouflage && (chicago->extra_flags & (1u << _shader_transparent_chicago_dont_fade_active_camouflage_bit)) == 0 )
        {
            float intensity = ((float)1.0 - group->effect.intensity);
            if ( intensity < 0.0f )
                intensity = 0.0f;
            else if ( intensity > 1.0f )
                intensity = 1.0f;
            color_constants[10] = (intensity * color_constants[10]);
        }
        if ( alpha_animation_index > 0 )
        {
            const render_animation *animation = group->animation;
            if ( animation && animation->values )
                color_constants[10] = animation->values[alpha_animation_index - 1] * color_constants[10];
        }
    }

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, color_constants, 3, (uint64_t)3 << 60);
    shader_transparent_chicago_create(group->shader, use_additional_op);
    rasterizer_transparent_geometry_group_draw_internal(group, 0);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0 /* D3DBLENDOP_ADD */);
}
