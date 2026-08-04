/* rasterizer_dx9_transparent_chicago_extended_draw @0x8382F028 — the shader_transparent_chicago_extended
 * variant of rasterizer_dx9_transparent_chicago_draw. Structurally identical to that sibling (bind vertex
 * shader/decl, recurse into extra shader layers, set transparent render state, optionally advance the
 * animated frame, then per texture stage bind the bitmap + sampler state and build the 8-float texture
 * transform, and finally upload both vertex-constant buffers, build the extended pixel-shader config, draw,
 * restore blend op). See rasterizer_dx9_transparent_chicago_draw.c for the full derivation of the shared
 * machinery — everything below reuses it verbatim except the four documented differences.
 *
 * Reconstructed from disassembly (0x8382F028-0x8382F830); the decompiler printed "local variable allocation
 * has failed". Same deviations as the chicago (non-extended) sibling: the __PAIR64__(bitmap_address_table_0,
 * __ROR4__(1,1)) is a fabrication (r25 = standalone 1<<63 from `li r9,1; extldi r25,r9,64,63` @0x8382F3B8/
 * F3C0, fused with the adjacent bitmap_address_table_0@ha array-base load); both SetVertexShaderConstantFN
 * calls are (0xD,buf,8,7<<58) and (0xA,buf,3,3<<60) per disasm 0x8382F724/F7F4; the sampler pokes are the
 * inlined named accessors (D3DDevice boundary type). Differences from the non-extended sibling, all verified:
 *   1. Uses the *_0 globals: _translation_table_0, bitmap_address_table_0 (int[4]={0,2,2,2} @0x8212A07C),
 *      bitmap_type_table_0 (u16[4]={0,2,2,2} @0x8212A074), and shader_transparent_chicago_extended_create.
 *   2. The chicago flags dword is at shader+0x6C (shader_tag[2].base.radiosity.tint_color.n[2]), NOT +0x60 —
 *      disasm 0x8382F21C (`&2` anim gate) and 0x8382F770 (`&1` effect gate). All other tag offsets match the
 *      non-extended sibling (flags@0x28, detail_level@0x2A, blend_fn@0x2C, alpha-anim-idx@0x30, extra count@
 *      0x48/table@0x4C, stage_count@0x54, stage_array@0x58; per-stage stride 220).
 *   3. A pre-loop first materialises the per-stage pointer array (stage_ptrs[i] = stage_array + 220*i,
 *      disasm 0x8382F374-F3A0); the main stage loop then iterates only stage_count times (not a fixed 4),
 *      so the sampler setup always runs.
 *   4. The final use_additional_op gate is the inverse of the sibling's: when (geometry_flags & (1u << _rasterizer_geometry_sky_bit)) and the
 *      blend halfword is zero, use_additional_op is set from the u/v-scale scratch's float bits (a shipped
 *      reinterpret / stale read; disasm 0x8382F7F0 `lwz r31, var_210`), otherwise it is 1 with the alpha calc. */

#include <stdint.h>
#include "headers/bitmap_group.h"
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/render_animation.h"
#include "headers/shader_texture_animation.h"
#include "headers/shader_transparent_chicago_map.h"
#include "headers/shader_transparent_chicago_map_flags.h"
#include "headers/shader_transparent_chicago_flags.h"
#include "headers/shader_transparent_chicago_extended.h"
#include "headers/real_vector4d.h"
#include "headers/real_matrix4x3.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/point2d.h"
#include "headers/global_frame_parameters.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_reference.h"
#include "headers/d3d_boundary.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/render_model_effect_type.h"


/* detail-level lookup tables (.rdata) — the "extended" copies */
#include "headers/shader_texture_animation.h"
#include "headers/point2d.h"
extern const int bitmap_address_table_0[4];             /* {0,2,2,2} @0x8212A07C */
extern const unsigned __int16 bitmap_type_table_0[4];   /* {0,2,2,2} @0x8212A074 */

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
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);

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
extern int shader_transparent_chicago_extended_create(const shader *shader, int use_additional_op);
extern double floor(double x);
extern void *memcpy(void *destination, const void *source, unsigned int size);

void rasterizer_dx9_transparent_chicago_extended_draw(const transparent_geometry_group *group, uint8_t dirty)
{
    __int16 vertex_shader_permutation = shader_get_vertex_shader_permutation(group->shader);
    __int16 primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);
    const shader *shader_tag = group->shader;   /* renamed from 'shader' to avoid shadowing the shader type */
    __int16 animation_frame_index = group->shader_permutation_index;
    int use_additional_op = 1;

    float color_constants[12] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    float texture_transform_constants[32];
    float uv_scale_scratch[2];   /* v68: last stage's {u_scale, v_scale}; also reinterpreted below */

    shader_transparent_chicago_map *stage_array =
        (shader_transparent_chicago_map *)*(int *)&shader_tag[2].base.radiosity.color.n[0];
    if ( !stage_array || !stage_array->map.name )
        return;

    D3DVertexShader *vshader =
        rasterizer_dx9_shaders_vshader9_get(_translation_table_0[6 * primary_vertex_type + vertex_shader_permutation]);
    D3DDevice_SetVertexShader(global_d3d_device, vshader);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(primary_vertex_type));
    D3DDevice_SetPixelShader(global_d3d_device, nullptr);

    int extra_layer_count = *(int *)&shader_tag[1].base.physics;   /* shader+0x48 */
    if ( extra_layer_count > 0 )
    {
        char *extra_layer_table = *(char **)&shader_tag[1].base.type;   /* shader+0x4C */
        for ( int i = 0; i < extra_layer_count; i = (__int16)(i + 1) )
        {
            transparent_geometry_group sub_group;
            memcpy(&sub_group, group, sizeof(sub_group));
            sub_group.sorted_index = -1;
            /* extra-layer block is a tag_reference[]; .index (@+0xC) selects the sub-shader tag */
            sub_group.shader = TAG_GET(const shader, ((tag_reference *)extra_layer_table)[i].index);
            rasterizer_transparent_geometry_group_draw(&sub_group, dirty);
        }
    }

    D3DDevice_SetRenderState_CullMode(global_d3d_device,
        (shader_tag[1].base.radiosity.flags & (1u << _shader_transparent_chicago_two_sided_bit)) != 0 ? 0 /* D3DCULL_NONE */ : 6 /* D3DCULL_CCW */);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, shader_tag[1].base.radiosity.flags & 1);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0x7F);
    rasterizer_set_framebuffer_blend_function((unsigned __int16)(*(unsigned int *)&shader_tag[1].base.radiosity.power >> 16));   /* shader+0x2C high halfword (BE) */

    if ( (shader_tag[1].base.radiosity.flags & (1u << _shader_transparent_chicago_numeric_bit)) != 0 && group->animation && *(int *)&shader_tag[2].base.radiosity.power > 0 )
    {
        int bitmap_frame_count = TAG_GET(bitmap_group, stage_array->map.index)->bitmaps.count;
        if ( (*(int *)&shader_tag[2].base.radiosity.tint_color.n[2] & 2) != 0 )   /* chicago_flags@0x6C & 2 */
        {
            animation_frame_index = numeric_countdown_timer_get(group->shader_permutation_index);
        }
        else
        {
            int frame_count = (__int16)bitmap_frame_count;
            int values_index = (bitmap_frame_count != 8) ? 0 : 3;
            /* typed: chicago_extended.numeric_counter_limit @ shader+0x28 (DB-verified) */
            float scale = (float)((const shader_transparent_chicago_extended *)shader_tag)->chicago_extended.numeric_counter_limit;
            int frame = (int)(float)floor((group->animation->values[values_index] * scale) + 0.5f);
            if ( frame < 0 )
                frame = 0;
            else if ( frame > (int)scale )
                frame = (int)scale;
            for ( int i = 0; i < group->shader_permutation_index; i = (__int16)(i + 1) )
                frame /= frame_count;
            animation_frame_index = frame % frame_count;
        }
    }

    int stage_count = (__int16)*(int *)&shader_tag[2].base.radiosity.power;

    /* materialise the per-stage pointer array before the main loop */
    shader_transparent_chicago_map *stage_ptrs[4];
    for ( int i = 0; i < stage_count; i = (__int16)(i + 1) )
        stage_ptrs[i] = &stage_array[i];

    for ( int stage = 0; stage < stage_count; stage = (__int16)(stage + 1) )
    {
        __int16 detail_level = shader_tag[1].base.radiosity.detail_level;
        shader_transparent_chicago_map *stage_ptr = stage_ptrs[stage];
        __int16 bitmap_type = stage ? 0 : bitmap_type_table_0[detail_level];
        rasterizer_set_texture(stage, bitmap_type, 0, stage_ptr->map.index, animation_frame_index);

        unsigned int address_u = (bitmap_type || (stage_ptr->flags & (1u << _shader_transparent_chicago_map_u_clamped_bit)) == 0)
                                     ? (stage ? 0 : bitmap_address_table_0[detail_level]) : 2;
        unsigned int address_v = (bitmap_type || (stage_ptr->flags & (1u << _shader_transparent_chicago_map_v_clamped_bit)) == 0)
                                     ? (stage ? 0 : bitmap_address_table_0[detail_level]) : 2;
        unsigned int address_w = stage ? 0 : bitmap_address_table_0[detail_level];
        unsigned int filter_flag = (stage_ptr->flags & (1u << _shader_transparent_chicago_map_point_sampled_bit)) == 0;

        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, stage, address_u);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, stage, address_v);
        D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, stage, address_w);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, stage, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, stage, filter_flag);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, stage, filter_flag);

        if ( stage == 0 && shader_tag[1].base.radiosity.detail_level )
        {
            /* first stage of a detail shader: pack env-map basis vectors, or the identity default */
            if ( (shader_tag[1].base.radiosity.flags & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0 )
            {
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
                texture_transform_constants[8 * stage + 0] = 1.0f;
                texture_transform_constants[8 * stage + 1] = 0.0f;
                texture_transform_constants[8 * stage + 2] = 0.0f;
                texture_transform_constants[8 * stage + 4] = 0.0f;
                texture_transform_constants[8 * stage + 5] = 1.0f;
                texture_transform_constants[8 * stage + 6] = 0.0f;
            }
        }
        else
        {
            /* texture-animation path */
            *(long long *)uv_scale_scratch = *(long long *)&stage_ptr->scale; /* {u_scale, v_scale} */
            float scale_u = uv_scale_scratch[0];
            float scale_v = uv_scale_scratch[1];
            int apply_base_map_scale = 1;
            if ( !stage && (shader_tag[1].base.radiosity.flags & (1u << _shader_transparent_chicago_scale_first_map_with_distance_bit)) != 0 )
            {
                scale_u = -(group->z_sort * uv_scale_scratch[0]);
                scale_v = -(group->z_sort * uv_scale_scratch[1]);
            }
            if ( (shader_tag[1].base.radiosity.flags & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0 )
                apply_base_map_scale = 0;
            if ( apply_base_map_scale )
            {
                scale_u = (group->model_base_map_scale.n[0] * scale_u);
                scale_v = (group->model_base_map_scale.n[1] * scale_v);
            }
            shader_texture_animation_evaluate(
                &stage_ptr->animation,
                group->animation, scale_u, scale_v,
                stage_ptr->offset.n[0], stage_ptr->offset.n[1], stage_ptr->rotation,
                (float)*(double *)&global_frame_parameters.game_time_sec,
                (real_vector4d *)&texture_transform_constants[8 * stage],
                (real_vector4d *)&texture_transform_constants[8 * stage + 4]);
        }
        texture_transform_constants[8 * stage + 7] = 0.0f;
        texture_transform_constants[8 * stage + 3] = 0.0f;
    }

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, texture_transform_constants, 8, (unsigned __int64)7 << 58);

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_sky_bit)) != 0 && !(unsigned __int16)(*(unsigned int *)&shader_tag[1].base.radiosity.power >> 16) )   /* shader+0x2C high halfword (BE) */
    {
        /* DEVIATION: shipped reinterpret — use_additional_op takes the u-scale scratch's float bits (v68[0]);
         * when stage_count==0 the scratch is never written, so this is a stale/uninitialized read. */
        use_additional_op = *(int *)&uv_scale_scratch[0];
    }
    else
    {
        use_additional_op = 1;
        __int16 alpha_animation_index = (unsigned __int16)(*(unsigned int *)&shader_tag[1].base.radiosity.color.n[0] >> 16);   /* shader+0x30 high halfword (BE) */

        if ( group->effect.type == _render_model_effect_type_active_camouflage && (*(int *)&shader_tag[2].base.radiosity.tint_color.n[2] & 1) == 0 )   /* @0x6C & 1 */
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

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, color_constants, 3, (unsigned __int64)3 << 60);
    shader_transparent_chicago_extended_create(group->shader, use_additional_op);
    rasterizer_transparent_geometry_group_draw_internal(group, 0);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0 /* D3DBLENDOP_ADD */);
}
