/* rasterizer_transparent_geometry_group_draw @0x83812BF8 — the top-level transparent-geometry draw dispatcher.
 * For one presorted geometry group it: (1) recurses to the previous presorted group; (2) runs the active-
 * camouflage source-object depth pre-fill for effect type 2; (3) runs the active-camouflage z-buffer fill for
 * type-4 shaders when required; (4) caches the primary render target for the multipass camouflage path; (5)
 * uploads node matrices / skinning / lighting; (6) sets the z-state; then (7) runs a two-pass loop that
 * switches on the shader base type and issues the matching D3D state + effect draw:
 *   _shader_type_effect (1)     — lightmap sampler setup + view/texture-transform vertex constants + effect draw
 *   _shader_type_model (4)      — active-camouflage draw
 *   _shader_type_transparent_chicago / _chicago_extended (6/7)
 *   _shader_type_transparent_water (8)
 *   _shader_type_transparent_glass (9)  — tint / reflection / diffuse sub-draws
 *   _shader_type_transparent_meter (0xA) — animated tint constants + pixel-shader constants + effect draw
 *   _shader_type_transparent_plasma (0xB) — plasma energy draw
 * Finally it recurses to the next presorted group and, if flagged, re-draws the active-camouflage groups.
 *
 * Reconstructed from disassembly (0x83812BF8-0x83814020); the decompiler printed "local variable allocation
 * has failed". Deviations:
 *   - The inline D3D fetch-constant register pokes (global_d3d_device->m_Constants.Fetch[0].Texture writes
 *     paired with m_Pending.m_Mask[3] |= 0x80000000) are decoded to the corpus's named inline sampler helpers
 *     per disasm (0x838134C8-0x83813578 case 1; 0x838137xx case 0xA; case 9 block): AddressU/AddressV (dword[0]
 *     rlwimi bits), MagFilter/MinFilter (explicit), SeparateZFilterEnable (dword[3] bit 8). Same convention as
 *     rasterizer_glass_draw_diffuse_pp.
 *   - SetVertexShaderConstantFN count/mask come from disasm (reg 0x1A: 3 / 3<<56; reg 0xD: 4 / 3<<59; reg 0xA:
 *     3 / 3<<60); the decompiler rendered them as garbage. SetPixelShaderConstantFN reg 0 count 6.
 *   - The `long double` cos/period scratch and the `__asm fsel` lines are plain double / (x>=0?x:1.0f).
 *   - The vertex-shader-constant scratch blocks are grouped into named float arrays; the decompiler's
 *     stacked-0x28-shader-layer puns (shader[N].base...) are retyped to the real derived tag structs
 *     (shader_effect / shader_transparent_glass / shader_transparent_meter, DB types_members-confirmed).
 *     local_last_source_object_index tracks the last drawn source object across the recursive calls. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/shader_effect.h"
#include "headers/shader_transparent_glass.h"
#include "headers/shader_transparent_meter.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/render_skinning.h"
#include "headers/render_animation.h"
#include "headers/bitmap_data.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector4d.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/global_frame_parameters.h"
#include "headers/shader_texture_animation.h"
#include "headers/d3d_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_type.h"
#include "headers/shader_effect_flags.h"
#include "headers/shader_transparent_glass_flags.h"
#include "headers/shader_transparent_meter_flags.h"
#include "headers/shader_transparent_glass_reflection_type.h"
#include "headers/periodic_function.h"
#include "headers/blam_data_globals.h"
#include "headers/render_model_effect_type.h"
#include "headers/_D3DTEXTURESTAGESTATETYPE.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_type.h"


/* D3D device state (thin wrappers over the command buffer) */
#include "headers/render_lighting.h"
#include "headers/point2d.h"
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value);

/* rasterizer / shader helpers */
extern uint8_t rasterizer_transparent_geometry_get_group_pending_status(const transparent_geometry_group *group);
extern void rasterizer_transparent_geometry_set_group_pending_status(const transparent_geometry_group *group, uint8_t status);
extern const transparent_geometry_group *rasterizer_transparent_geometry_get_group_from_presorted_index(int16_t group_presorted_index);
extern const transparent_geometry_group *rasterizer_transparent_geometry_next_group(const transparent_geometry_group *group);
extern const transparent_geometry_group *rasterizer_transparent_geometry_get_groups2(int16_t *count);
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);
extern void rasterizer_transparente_geometry_group_draw_zbuffer(const transparent_geometry_group *group);
extern uint8_t shader_ignores_effect(const shader *shader);
extern uint8_t shader_is_decal(const shader *shader);
extern uint8_t shader_is_water_decal(const shader *shader);
extern int16_t shader_get_vertex_shader_permutation(const shader *shader);
extern void shader_texture_animation_evaluate(const shader_texture_animation *texture_animation, const render_animation *render_animation, float u_scale, float v_scale, float u_offset, float v_offset, float r_offset, float time_value, real_vector4d *u_transform_reference, real_vector4d *v_transform_reference);
extern void rasterizer_set_model_skinning(const render_skinning *skinning, uint8_t uploadconstants);
extern void _rasterizer_model_setupnodeparts(int num_nodes, const uint8_t *node_table, real_matrix4x3 *node_matrices);
extern void rasterizer_set_model_lighting(const render_lighting *lighting);
extern void rasterizer_set_frustum_z(float z_near, float z_far);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_set_framebuffer_blend_function(int16_t framebuffer_blend_function);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void rasterizer_dx9_set_transparent_decal_zbias(void);
extern void rasterizer_dx9_reset_zbias(void);
extern uint8_t rasterizer_active_camouflage_require_zbuffer_fill(void);
extern void rasterizer_active_camouflage_cache_primary_render_target(void);
extern void rasterizer_active_camouflage_draw(const transparent_geometry_group *group);
extern void rasterizer_dx9_transparent_chicago_draw(const transparent_geometry_group *group, uint8_t dirty);
extern void rasterizer_dx9_transparent_chicago_extended_draw(const transparent_geometry_group *group, uint8_t dirty);
/* deviation: rasterizer_water_draw/glass_draw_* are function-pointer globals (blam_data_globals.h) */
extern void rasterizer_plasma_energy_draw(const transparent_geometry_group *group);
extern float periodic_function_evaluate(int16_t function_type, float time);

void rasterizer_transparent_geometry_group_draw(const transparent_geometry_group *group, uint8_t dirty)
{
    unsigned char camo_second_pass = 0;

    if ( (group->active_camouflage_transparent_source_object_index && !dirty)
         || !rasterizer_transparent_geometry_get_group_pending_status(group) )
    {
        return;
    }
    rasterizer_transparent_geometry_set_group_pending_status(group, 0);

    int prev_presorted_index = (uint16_t)group->prev_group_presorted_index;
    if ( prev_presorted_index != 0xFFFF )
        rasterizer_transparent_geometry_group_draw(
                rasterizer_transparent_geometry_get_group_from_presorted_index(prev_presorted_index), dirty);

    int *last_source = &local_last_source_object_index;

    /* type-2 active-camouflage source-object depth pre-fill */
    if ( (uint16_t)group->effect.type == _render_model_effect_type_transparent_zbuffered )
    {
        int source_object_index = group->source_object_index;
        if ( source_object_index != local_last_source_object_index && !dirty )
        {
            const transparent_geometry_group *g = group;
            D3DDevice_SetRenderState_CullMode(global_d3d_device, 0 /* D3DCULL_NONE */);
            D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0);
            D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
            D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
            D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3 /* D3DCMP_LESSEQUAL */);
            D3DDevice_SetPixelShader(global_d3d_device, nullptr);
            SetTextureStageStateSmart(0, D3DTSS_COLOROP, 2);
            SetTextureStageStateSmart(0, D3DTSS_COLORARG1, 3);
            SetTextureStageStateSmart(0, D3DTSS_ALPHAOP, 2);
            SetTextureStageStateSmart(0, D3DTSS_ALPHAARG1, 3);
            SetTextureStageStateSmart(1, D3DTSS_COLOROP, 1);
            SetTextureStageStateSmart(1, D3DTSS_ALPHAOP, 1);
            D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_model));
            D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_model_zbuffer));
            do
            {
                if ( g->source_object_index != source_object_index || g->effect.type != _render_model_effect_type_transparent_zbuffered )
                    break;
                if ( !shader_ignores_effect(g->shader) )
                {
                    render_skinning skinning;
                    real_matrix4x3 *node_matrices;
                    if ( g->node_matrices && g->node_matrix_count )
                    {
                        skinning.node_matrix_count = g->node_matrix_count;
                        node_matrices = (real_matrix4x3 *)g->node_matrices;
                    }
                    else
                    {
                        node_matrices = (real_matrix4x3 *)global_identity4x3;
                        skinning.node_matrix_count = 1;
                    }
                    skinning.node_matrices = node_matrices;
                    rasterizer_set_model_skinning(&skinning,
                            ((uint16_t)~(uint16_t)(group->geometry_flags >> 16) >> 8) & 1);
                    if ( (group->geometry_flags & (1u << _rasterizer_geometry_parts_define_local_nodes_bit)) != 0 )
                        _rasterizer_model_setupnodeparts(g->local_node_remap_table_size,
                                g->local_node_remap_table, skinning.node_matrices);
                    if ( group->lighting )
                        rasterizer_set_model_lighting(g->lighting);
                    rasterizer_transparent_geometry_group_draw_internal(g, 0);
                }
                g = rasterizer_transparent_geometry_next_group(g);
            }
            while ( g );
        }
    }

    /* active-camouflage z-buffer fill for type-4 shaders */
    if ( rasterizer_active_camouflage_require_zbuffer_fill()
         && (group->geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) == 0 && group->effect.type == _render_model_effect_type_active_camouflage )
    {
        int source_object_index = group->source_object_index;
        if ( source_object_index != local_last_source_object_index )
        {
            const shader *shader = group->shader;
            if ( shader && shader->base.type == _shader_type_model && !dirty )
            {
                const transparent_geometry_group *g = group;
                D3DDevice_SetRenderState_CullMode(global_d3d_device, 6 /* D3DCULL_CCW */);
                D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0);
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
                D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
                D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3 /* D3DCMP_LESSEQUAL */);
                D3DDevice_SetPixelShader(global_d3d_device, nullptr);
                SetTextureStageStateSmart(0, D3DTSS_COLOROP, 2);
                SetTextureStageStateSmart(0, D3DTSS_COLORARG1, 3);
                SetTextureStageStateSmart(0, D3DTSS_ALPHAOP, 2);
                SetTextureStageStateSmart(0, D3DTSS_ALPHAARG1, 3);
                SetTextureStageStateSmart(1, D3DTSS_COLOROP, 1);
                SetTextureStageStateSmart(1, D3DTSS_ALPHAOP, 1);
                do
                {
                    if ( g->source_object_index != source_object_index || g->effect.type != _render_model_effect_type_active_camouflage )
                        break;
                    if ( g->shader )
                        rasterizer_transparente_geometry_group_draw_zbuffer(g);
                    g = rasterizer_transparent_geometry_next_group(g);
                }
                while ( g );
                D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
            }
        }
    }

    /* cache the primary render target for the multipass camouflage path */
    if ( (group->geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) == 0 && global_window_parameters.rasterizer_target == _rasterizer_target_render_primary && !dirty )
    {
        unsigned char do_cache = 0;
        if ( rasterizer_debug_options.active_camouflage_multipass_enabled )
        {
            const shader *shader = group->shader;
            if ( shader && shader->base.type == _shader_type_model && group->effect.type == _render_model_effect_type_active_camouflage
                 && group->source_object_index != local_last_source_object_index )
                do_cache = 1;
        }
        else
        {
            const shader *shader = group->shader;
            if ( !(shader && (shader->base.type == _shader_type_transparent_water || shader_is_water_decal(group->shader))) )
                do_cache = 1;
        }
        if ( do_cache )
            rasterizer_active_camouflage_cache_primary_render_target();
    }

    /* decide whether a second camouflage pass is needed after this group */
    if ( (group->geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) == 0 && global_window_parameters.rasterizer_target == _rasterizer_target_render_primary
         && group->effect.type == _render_model_effect_type_active_camouflage )
    {
        const shader *group_shader = group->shader; /* deviation: renamed local (shadowed typedef 'shader') */
        if ( group_shader && group_shader->base.type == _shader_type_model && !dirty )
        {
            const transparent_geometry_group *next = rasterizer_transparent_geometry_next_group(group);
            const shader *next_shader;
            if ( !next || next->effect.type != _render_model_effect_type_active_camouflage || next->source_object_index != group->source_object_index
                 || (next_shader = next->shader) == nullptr || next_shader->base.type != _shader_type_model )
            {
                camo_second_pass = 1;
            }
        }
    }

    if ( !group->shader )
    {
        group->render_proc(group->first_triangle_index, group->triangle_count);  /* widget path: union arm, no cast */
        goto record_source;
    }

    shader_get_vertex_shader_permutation(group->shader);
    int16_t primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) == 0 )
    {
        render_skinning skinning;
        real_matrix4x3 *node_matrices;
        if ( group->node_matrices && group->node_matrix_count )
        {
            skinning.node_matrix_count = group->node_matrix_count;
            node_matrices = (real_matrix4x3 *)group->node_matrices;
        }
        else
        {
            node_matrices = (real_matrix4x3 *)global_identity4x3;
            skinning.node_matrix_count = 1;
        }
        skinning.node_matrices = node_matrices;
        rasterizer_set_model_skinning(&skinning, ((uint16_t)~(uint16_t)(group->geometry_flags >> 16) >> 8) & 1);
        if ( (group->geometry_flags & (1u << _rasterizer_geometry_parts_define_local_nodes_bit)) != 0 )
            _rasterizer_model_setupnodeparts(group->local_node_remap_table_size,
                    group->local_node_remap_table, skinning.node_matrices);
        if ( group->lighting )
            rasterizer_set_model_lighting(group->lighting);
    }

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_no_zbuffer_bit)) != 0 )
    {
        if ( global_window_parameters.rasterizer_target == _rasterizer_target_render_primary )
            rasterizer_set_frustum_z(0.00390625f, 4096.0f);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
    }
    else
    {
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3 /* D3DCMP_LESSEQUAL */);
        if ( shader_is_decal(group->shader) )
            rasterizer_dx9_set_transparent_decal_zbias();
        else
            rasterizer_dx9_reset_zbias();
    }

    int pass = 0;
    do
    {
        if ( (group->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 )
        {
            if ( group->effect.type == _render_model_effect_type_active_camouflage )
            {
                if ( (int16_t)pass > 0 )
                    break;
                rasterizer_set_frustum_z(rasterizer_globals.z_near_first_person,
                                         rasterizer_globals.z_far_first_person);
            }
            else if ( (int16_t)pass )
            {
                rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
                D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
            }
            else
            {
                const shader *s = group->shader;
                if ( s && s->base.type == _shader_type_effect
                     && (((const shader_effect *)s)->effect.flags & (1u << _shader_effect_flags_dont_overdraw_first_person_weapon_bit)) != 0 )
                    goto next_pass;
                rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject_invert);
            }
        }
        else if ( (int16_t)pass > 0 )
        {
            break;
        }

        const shader *shader = group->shader;
        rasterizer_dx9_shader *effect_shader;
        switch ( shader->base.type )
        {
            case _shader_type_effect:
            {
                /* DEVIATION: the decompiler modeled the tag as stacked 0x28 `shader` layers
                 * (shader[N].base...); retyped to the real shader_effect tag fields. */
                const shader_effect *fx = (const shader_effect *)shader;
                char has_texture_animation;
                if ( fx->effect.secondary_map.index == -1
                     || (uint16_t)fx->effect.secondary_map_anchor == 2 )
                    has_texture_animation = 0;
                else
                    has_texture_animation = 1;

                /* uses_nonlinear_tint selects the +96.. dx-shader family, otherwise the family 6 lower
                 * (compiler form was flag_bias = 0 or -1; (-1 & 0xFFFA) == -6). */
                int16_t flag_bias = ((fx->effect.flags & (1u << _shader_effect_uses_nonlinear_tint_bit)) != 0) ? 0 : -6;
                int16_t shader_index = flag_bias + 96;
                unsigned int detail_level;
                if ( (group->geometry_flags & (1u << _rasterizer_geometry_no_fog_bit)) != 0 )
                    goto env_shader_selected;
                detail_level = (uint16_t)fx->effect.framebuffer_blend_function;
                if ( detail_level > 7 )
                    goto env_shader_selected;
                if ( detail_level == 1 )
                {
                    shader_index = flag_bias + 100;
                }
                else if ( detail_level == 2 )
                {
                    shader_index = flag_bias + 99;
                }
                else if ( (detail_level == 3 || detail_level == 4) && fx->effect.framebuffer_blend_function )
                {
                    shader_index = flag_bias + 97;
                }
                else if ( detail_level == 5 && fx->effect.framebuffer_blend_function )
                {
                    shader_index = flag_bias + 100;
                }
                else if ( detail_level == 6 && fx->effect.framebuffer_blend_function )
                {
                    shader_index = flag_bias + 97;
                }
                else if ( fx->effect.framebuffer_blend_function )
                {
                    shader_index = flag_bias + 101;
                }
                else
                {
                    shader_index = flag_bias + 98;
                }
env_shader_selected:
                effect_shader = rasterizer_shader_select(shader_index);
                if ( effect_shader && effect_shader->effect )
                {
                    const bitmap_data *lightmap = group->lightmap;
                    if ( lightmap && lightmap->hardware_format )
                    {
                        /* DEVIATION: the decompiler read one 32-bit word covering framebuffer_fade_mode
                         * (high half, BE) + primary_map_flags (low half); untangled into the two fields.
                         * The SeparateZFilterEnable bit came from bit 23 of that packed word = bit 7 of
                         * framebuffer_fade_mode (always 0 for valid fade modes, so the value is 1). */
                        uint16_t primary_map_flags = fx->effect.primary_map_flags;
                        rasterizer_set_texture_bitmap_data_for_effect(0, (bitmap_data *)group->lightmap, effect_shader); /* lazy hw-format mutation: drop const view */
                        /* DEVIATION: fetch-constant sampler pokes decoded to inline helpers (0x838134C8-). */
                        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, ((unsigned int)primary_map_flags << 10) & 0x800);
                        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, ((unsigned int)primary_map_flags << 12) & 0x4000);
                        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, (primary_map_flags & 1) == 0);
                        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, (primary_map_flags & 1) == 0);
                        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0,
                                (~(unsigned int)(uint16_t)fx->effect.framebuffer_fade_mode >> 7) & 1);
                    }
                    D3DDevice_SetRenderState_CullMode(global_d3d_device, 0 /* D3DCULL_NONE */);
                    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
                    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
                    rasterizer_set_framebuffer_blend_function(fx->effect.framebuffer_blend_function);

                    /* view-to-world + camera position (register 0x1A, 3 vec4) */
                    float camera_constants[12];
                    camera_constants[0] = 1.0f; camera_constants[1] = 0.0f; camera_constants[2] = 0.0f;
                    camera_constants[3] = 0.0f; camera_constants[4] = 0.0f; camera_constants[5] = 1.0f;
                    camera_constants[6] = 0.0f; camera_constants[7] = 0.0f; camera_constants[8] = 0.0f;
                    camera_constants[9] = 0.0f; camera_constants[10] = 1.0f; camera_constants[11] = 0.0f;
                    if ( (group->geometry_flags & (1u << _rasterizer_geometry_viewspace_bit)) != 0 )
                    {
                        camera_constants[0] = global_window_parameters.frustum.view_to_world.n[0][0];
                        camera_constants[1] = global_window_parameters.frustum.view_to_world.n[1][0];
                        camera_constants[2] = global_window_parameters.frustum.view_to_world.n[2][0];
                        camera_constants[3] = global_window_parameters.camera.position.n[0];
                        camera_constants[4] = global_window_parameters.frustum.view_to_world.n[0][1];
                        camera_constants[5] = global_window_parameters.frustum.view_to_world.n[1][1];
                        camera_constants[6] = global_window_parameters.frustum.view_to_world.n[2][1];
                        camera_constants[7] = global_window_parameters.camera.position.n[1];
                        camera_constants[8] = global_window_parameters.frustum.view_to_world.n[0][2];
                        camera_constants[9] = global_window_parameters.frustum.view_to_world.n[1][2];
                        camera_constants[10] = global_window_parameters.frustum.view_to_world.n[2][2];
                        camera_constants[11] = global_window_parameters.camera.position.n[2];
                    }

                    /* texture transform (register 0xD, 4 vec4) */
                    float texture_transform[16];
                    texture_transform[0] = 1.0f; texture_transform[1] = 0.0f; texture_transform[2] = 0.0f;
                    texture_transform[3] = 0.0f; texture_transform[4] = 0.0f; texture_transform[5] = 1.0f;
                    texture_transform[6] = 0.0f; texture_transform[7] = 0.0f; texture_transform[8] = 0.0f;
                    texture_transform[9] = 0.0f; texture_transform[10] = 0.0f; texture_transform[11] = 0.0f;
                    texture_transform[12] = 0.0f; texture_transform[13] = 0.0f; texture_transform[14] = 0.0f;
                    texture_transform[15] = 0.0f;
                    if ( has_texture_animation )
                    {
                        real_vector4d *out0 = (real_vector4d *)&texture_transform[8];
                        real_vector4d *out1 = (real_vector4d *)&texture_transform[12];
                        shader_texture_animation_evaluate(
                                &fx->effect.secondary_map_animation,
                                group->animation, group->model_base_map_scale.n[0],
                                group->model_base_map_scale.n[1], 0.0f, 0.0f, 0.0f,
                                (float)global_frame_parameters.game_time_sec, out1, out0);
                    }

                    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x1A, camera_constants, 3,
                                                        (uint64_t)3 << 56);
                    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, texture_transform, 4,
                                                        (uint64_t)3 << 59);
                    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_unlit));
                    D3DDevice_SetVertexShader(global_d3d_device,
                            rasterizer_dx9_shaders_vshader9_get(effect_shader->vshader9));
                    unsigned int pass_count;
                    ID3DXEffect_Begin(effect_shader->effect, &pass_count, 3);
                    for ( unsigned int i = 0; i < pass_count; ++i )
                    {
                        ID3DXEffect_BeginPass(effect_shader->effect, i);
                        rasterizer_transparent_geometry_group_draw_internal(group, 0);
                        ID3DXEffect_EndPass(effect_shader->effect);
                    }
                    ID3DXEffect_End(effect_shader->effect);
                }
                break;
            }

            case _shader_type_model:
                if ( group->effect.type != _render_model_effect_type_active_camouflage )
                    break;
                if ( ((unsigned char *)last_source)[4] )
                    return;
                rasterizer_active_camouflage_draw(group);
                break;

            case _shader_type_transparent_chicago:
                rasterizer_dx9_transparent_chicago_draw(group, dirty);
                break;

            case _shader_type_transparent_chicago_extended:
                rasterizer_dx9_transparent_chicago_extended_draw(group, dirty);
                break;

            case _shader_type_transparent_water:
                rasterizer_water_draw(group);
                break;

            case _shader_type_transparent_glass:
            {
                /* DEVIATION: the decompiler modeled the tag as stacked 0x28 `shader` layers
                 * (shader[N].base...); retyped to the real shader_transparent_glass tag fields. */
                const shader_transparent_glass *glass = (const shader_transparent_glass *)shader;
                /* reflection_mode holds the glass reflection.type tag field (bumped/flat/mirror);
                 * it is later overwritten with a computed permutation index for the draw call. */
                int16_t reflection_mode = glass->glass.reflection_type;
                D3DDevice_SetRenderState_CullMode(global_d3d_device,
                        (glass->glass.flags & (1u << _shader_transparent_glass_two_sided_bit)) != 0 ? 0 /* D3DCULL_NONE */ : 6 /* D3DCULL_CCW */);
                D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0 /* D3DBLENDOP_ADD */);
                D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
                /* DEVIATION: fetch-constant sampler pokes decoded to inline helpers. */
                D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
                D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 0);
                D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
                D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
                D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
                if ( reflection_mode != _shader_transparent_glass_reflection_type_mirror
                     || (global_window_parameters.has_mirror
                         && (reflection_mode != _shader_transparent_glass_reflection_type_mirror || global_window_parameters.rasterizer_target == _rasterizer_target_render_primary)) )
                {
                    if ( glass->glass.tint_map.index != -1
                         || glass->glass.tint_color.n[0] != 0.0f
                         || glass->glass.tint_color.n[1] != 0.0f
                         || glass->glass.tint_color.n[2] != 0.0f )
                    {
                        rasterizer_glass_draw_tint(group);
                    }
                    if ( (glass->glass.reflection_view_perpendicular_color.alpha > 0.0f
                          || glass->glass.reflection_view_parallel_color.alpha > 0.0f)
                         && (glass->glass.reflection_map.index != -1
                             || reflection_mode == _shader_transparent_glass_reflection_type_mirror) )
                    {
                        if ( !reflection_mode )
                            reflection_mode = (glass->glass.flags & (1u << _shader_transparent_glass_bump_map_is_specular_mask_bit)) != 0;
                        if ( !reflection_mode )
                            reflection_mode = glass->glass.reflection_bump_map.index == -1;
                        rasterizer_glass_draw_reflection(group, reflection_mode);
                    }
                    if ( glass->glass.diffuse_map.index != -1
                         || glass->glass.diffuse_detail_map.index != -1 )
                        rasterizer_glass_draw_diffuse(group);
                }
                break;
            }

            case _shader_type_transparent_meter:
            {
                int16_t vshader_bias = 0;
                if ( !primary_vertex_type || primary_vertex_type == _rasterizer_vertex_type_environment_lightmap_uncompressed )
                    vshader_bias = 0;
                else if ( primary_vertex_type == _rasterizer_vertex_type_model_uncompressed )
                    vshader_bias = 1;
                effect_shader = rasterizer_shader_select(_dxshader_transparent_meter);
                if ( !effect_shader || !effect_shader->effect )
                    break;
                /* DEVIATION: the decompiler modeled the tag as stacked 0x28 `shader` layers
                 * (s[N].base...); retyped to the real shader_transparent_meter tag fields. */
                const shader_transparent_meter *mtr = (const shader_transparent_meter *)group->shader;
                float anim_a = 1.0f, anim_b = 1.0f, anim_c = 1.0f, anim_d = 1.0f;
                D3DDevice_SetVertexDeclaration(global_d3d_device,
                        rasterizer_dx9_shaders_vdecl9_get(primary_vertex_type));
                D3DDevice_SetVertexShader(global_d3d_device,
                        rasterizer_dx9_shaders_vshader9_get(vshader_bias + 57));
                const render_animation *animation = group->animation;
                if ( animation )
                {
                    const float *values = animation->values;
                    if ( values )
                    {
                        int idx_a = mtr->meter.meter_brightness_source;
                        if ( idx_a >= 1 && idx_a <= 4 )
                            anim_a = values[idx_a - 1];
                        int idx_b = mtr->meter.flash_brightness_source;
                        if ( idx_b >= 1 && idx_b <= 4 )
                            anim_b = values[idx_b - 1];
                        int idx_c = mtr->meter.value_source;
                        if ( idx_c >= 1 && idx_c <= 4 )
                            anim_c = values[idx_c - 1];
                        int idx_d = mtr->meter.gradient_source;
                        if ( idx_d >= 1 && idx_d <= 4 )
                            anim_d = values[idx_d - 1];
                    }
                }
                if ( rasterizer_debug_options.debug_meter_shader_enabled )
                {
                    periodic_function_evaluate(_periodic_function_cosine,
                            global_frame_parameters.game_time_sec / rasterizer_debug_options.pad3_scale);
                    anim_a = rasterizer_debug_options.f[0] >= 0.0f ? rasterizer_debug_options.f[0] : 1.0f;
                    anim_b = rasterizer_debug_options.f[1] >= 0.0f ? rasterizer_debug_options.f[1] : 1.0f;
                    anim_c = rasterizer_debug_options.f[2] >= 0.0f ? rasterizer_debug_options.f[2] : 1.0f;
                    anim_d = rasterizer_debug_options.f[3] >= 0.0f ? rasterizer_debug_options.f[3] : 1.0f;
                }

                float scaled_intensity = (anim_d * 8.0f);
                if ( scaled_intensity <= 1.0f )
                    scaled_intensity = 1.0f;

                /* pixel-shader constants (register 0, 6 vec4 = 24 floats) */
                float pixel_constants[24];
                pixel_constants[0] = mtr->meter.flash_color.n[0] * anim_b;
                pixel_constants[1] = mtr->meter.flash_color.n[1] * anim_b;
                pixel_constants[2] = mtr->meter.flash_color.n[2] * anim_b;
                pixel_constants[3] = 1.0f;
                pixel_constants[4] = mtr->meter.gradient_max_color.n[0];
                pixel_constants[5] = mtr->meter.gradient_max_color.n[1];
                pixel_constants[6] = mtr->meter.gradient_max_color.n[2];
                pixel_constants[7] = (1.0f / scaled_intensity);
                pixel_constants[8] = mtr->meter.gradient_min_color.n[0];
                pixel_constants[9] = mtr->meter.gradient_min_color.n[1];
                pixel_constants[10] = mtr->meter.gradient_min_color.n[2];
                pixel_constants[11] = anim_c;
                pixel_constants[12] = mtr->meter.background_color.n[0];
                pixel_constants[13] = mtr->meter.background_color.n[1];
                pixel_constants[14] = mtr->meter.background_color.n[2];
                pixel_constants[15] = 1.0f;
                pixel_constants[16] = mtr->meter.tint_color.n[0];
                pixel_constants[17] = mtr->meter.tint_color.n[1];
                pixel_constants[18] = mtr->meter.tint_color.n[2];
                pixel_constants[19] = 1.0f;
                if ( (mtr->meter.flags & (1u << _shader_transparent_meter_flash_color_is_negative_bit)) != 0 )
                {
                    pixel_constants[23] = -1.0f;
                    pixel_constants[20] = -(mtr->meter.flash_color.n[0] * anim_b);
                    pixel_constants[21] = -(mtr->meter.flash_color.n[1] * anim_b);
                    pixel_constants[22] = -(mtr->meter.flash_color.n[2] * anim_b);
                }
                else
                {
                    pixel_constants[23] = 1.0f;
                    pixel_constants[20] = (mtr->meter.flash_color.n[0] * anim_b);
                    pixel_constants[21] = (mtr->meter.flash_color.n[1] * anim_b);
                    pixel_constants[22] = (mtr->meter.flash_color.n[2] * anim_b);
                }
                if ( (mtr->meter.flags & (1u << _shader_transparent_meter_tint_mode_2_bit)) != 0 )
                {
                    pixel_constants[3] = mtr->meter.meter_transparency;
                    pixel_constants[17] = (mtr->meter.tint_color.n[1] * anim_a);
                    pixel_constants[18] = (mtr->meter.tint_color.n[2] * anim_a);
                    pixel_constants[15] = mtr->meter.background_transparency;
                    pixel_constants[16] = (mtr->meter.tint_color.n[0] * anim_a);
                    pixel_constants[19] = mtr->meter.meter_transparency;
                }
                else
                {
                    pixel_constants[3] = anim_a;
                    pixel_constants[15] = 0.0f;
                    pixel_constants[16] = mtr->meter.tint_color.n[0];
                    pixel_constants[17] = mtr->meter.tint_color.n[1];
                    pixel_constants[18] = mtr->meter.tint_color.n[2];
                    pixel_constants[19] = anim_a;
                }

                rasterizer_set_texture_for_effect(0, 0, 1, mtr->meter.map.index,
                        group->shader_permutation_index, effect_shader);
                /* DEVIATION: fetch-constant sampler pokes decoded to inline helpers. */
                D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
                D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 0);
                D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0,
                        ((unsigned int)~mtr->meter.flags >> _shader_transparent_meter_point_sampled_bit) & 1);
                D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0,
                        ((unsigned int)~mtr->meter.flags >> _shader_transparent_meter_point_sampled_bit) & 1);
                D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0,
                        (~((unsigned int)mtr->meter.flags >> 4) >> 23) & 1);
                D3DDevice_SetRenderState_CullMode(global_d3d_device,
                        (mtr->meter.flags & (1u << _shader_transparent_meter_two_sided_bit)) != 0 ? 0 /* D3DCULL_NONE */ : 6 /* D3DCULL_CCW */);
                D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1 /* D3DBLEND_ONE */);
                D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1 /* D3DBLEND_ONE */);
                D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0 /* D3DBLENDOP_ADD */);
                D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);

                /* base-map transform (register 0xA, 3 vec4) */
                float base_map_transform[12];
                base_map_transform[0] = 1.0f; base_map_transform[1] = 1.0f; base_map_transform[2] = 1.0f;
                base_map_transform[3] = 1.0f;
                base_map_transform[4] = group->model_base_map_scale.n[0];
                base_map_transform[5] = 0.0f; base_map_transform[6] = 0.0f; base_map_transform[7] = 0.0f;
                base_map_transform[8] = 0.0f;
                base_map_transform[9] = group->model_base_map_scale.n[1];
                base_map_transform[10] = 0.0f; base_map_transform[11] = 0.0f;
                D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, base_map_transform, 3,
                                                    (uint64_t)3 << 60);
                if ( rasterizer_debug_options.debug_meter_shader_enabled && rasterizer_debug_options.pad3 )
                    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
                unsigned int pass_count;
                ID3DXEffect_Begin(effect_shader->effect, &pass_count, 3);
                for ( unsigned int j = 0; j < pass_count; ++j )
                {
                    ID3DXEffect_BeginPass(effect_shader->effect, j);
                    D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 6,
                                                       (uint64_t)3 << 62);
                    rasterizer_transparent_geometry_group_draw_internal(group, 0);
                    ID3DXEffect_EndPass(effect_shader->effect);
                }
                ID3DXEffect_End(effect_shader->effect);
                break;
            }

            case _shader_type_transparent_plasma:
                rasterizer_plasma_energy_draw(group);
                break;

            default:
                break;
        }
next_pass:
        pass = (int16_t)(pass + 1);
    }
    while ( pass < 2 );

    if ( (group->geometry_flags & (1u << _rasterizer_geometry_no_zbuffer_bit)) != 0 && global_window_parameters.rasterizer_target == _rasterizer_target_render_primary )
        rasterizer_set_frustum_z(0.0f, 0.0f);
    if ( (group->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 && group->effect.type == _render_model_effect_type_active_camouflage )
        rasterizer_set_frustum_z(0.0f, 0.0f);
    rasterizer_dx9_reset_zbias();

record_source:
    if ( !dirty )
        *last_source = group->source_object_index;

    int next_presorted_index = (uint16_t)group->next_group_presorted_index;
    if ( next_presorted_index != 0xFFFF )
        rasterizer_transparent_geometry_group_draw(
                rasterizer_transparent_geometry_get_group_from_presorted_index(next_presorted_index), dirty);

    if ( camo_second_pass )
    {
        int16_t group_count;
        const transparent_geometry_group *groups2 = rasterizer_transparent_geometry_get_groups2(&group_count);
        for ( int k = 0; k < group_count; k = (int16_t)(k + 1) )
        {
            const transparent_geometry_group *g = &groups2[k];
            if ( g->active_camouflage_transparent_source_object_index == group->source_object_index
                 && g->effect.type == _render_model_effect_type_active_camouflage )
            {
                uint8_t camo_dirty = 1; /* second camouflage pass draws dirty */
                rasterizer_transparent_geometry_group_draw(g, camo_dirty);
                if ( rasterizer_debug_options.pad3 )
                    ((unsigned char *)last_source)[4] = 1;
            }
        }
    }
}
