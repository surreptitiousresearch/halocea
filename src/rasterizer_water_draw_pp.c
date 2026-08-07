/* rasterizer_water_draw_pp @0x83787EE0 — post-process water-surface draw for one transparent geometry group.
 *
 * Gated on render targets being enabled and the draw_water debug option. Maps the group's primary vertex type
 * to a water shader variant, then:
 *  - For an unsupported vertex type (not 0/2/4): a plain fixed-function textured pass (the `else` fallback).
 *  - Otherwise the full water path: optionally rebuilds the animated bump map, then draws up to three effect
 *    passes — shader 102 pass 0 (reflection/normalization, gated on shader flag bit0), shader 102 pass 1
 *    (refraction blend, gated on flag bit1), and shader 103 (the composited water surface with a per-pass
 *    reflection tint that fades between the shader's two tint colors by how edge-on the water plane is to the
 *    view). Bump/scroll animation constants come from the shader's animation fields and the frame game time.
 *
 * DEVIATIONS (decompiler emitted "local variable allocation has failed"; puns resolved against disasm
 * 0x83787EE0-0x837889B0):
 *  - Per-sampler m_Constants.Fetch[N]/m_Pending.m_Mask pokes restored to named inlined sampler-state helpers
 *    (plasma_energy_draw / active_camouflage_draw convention). `__ROL4__(1,11)&0x1C00` == 2<<10 => AddressU
 *    Value 2 (CLAMP); `__ROL4__(1,14)&0xE000` == AddressV Value 2; `__ROR4__(1,15)&0x70000` == 2<<16 =>
 *    AddressW Value 2; `&= 0xFFFFE3FF` / `&= 0xFFFF1FFF` are AddressU/AddressV Value 0 (WRAP); dword[3] bit23
 *    poke is SeparateZFilterEnable Value 1.
 *  - The inline m_Constants.Alu[256] float4 poke (+ m_Pending.m_Mask[1]) is pixel-shader constant c256,
 *    expressed as D3DDevice_SetPixelShaderConstantFN(...,256,...,1,...) per the same convention.
 *  - The mask register `v13` the decompiler read as `__ROR4__(1,1)` (0x80000000) is really (u64)1<<63
 *    (extldi r25,1,64,63); it is the PendingMask for every PS-constant / Alu[256] write here.
 *  - SetVertexShaderConstantFN(0xA,...) count/mask (rendered 0x30000000u / v44) are `li r6,3` / `sldi r7,r7,60`
 *    => count 3, mask (u64)3<<60.
 *  - Garbled call args resolved from disasm: set_texture_direct_for_effect stage = 1 (li r3,1), the second
 *    set_texture_for_effect stage = 3 with dxeffect = shader-103 effect, all MagFilter values = 1.
 *  - The `v5 = HIWORD(v66[0])` default branch reads an uninitialized stack slot (dead: water vertex types are
 *    only 0/2/4); modeled as the 0 case. */

#include <stdint.h>
#include <math.h>

#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/shader_transparent_water.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_frame_begin_parameters.h"
#include "headers/real_plane3d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_transparent_water_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/point2d.h"
extern float __fsqrts(float);


/* cos provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* sin provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern void rasterizer_water_build_bumpmap(const shader *shader);
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);

extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask1);

/* Clamp-address + point-filter one sampler (compiler-inlined sampler-state setters). */
static void water_clamp_sampler(unsigned int sampler)
{
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, sampler, 2);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, sampler, 2);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, sampler, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, sampler, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, sampler, 1);
}

/* As above, plus AddressW clamp (for the 3D normalization / bump samplers). */
static void water_clamp_sampler_w(unsigned int sampler)
{
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, sampler, 2);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, sampler, 2);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, sampler, 2);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, sampler, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, sampler, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, sampler, 1);
}

void rasterizer_water_draw_pp(const transparent_geometry_group *group)
{
    if ( rasterizer_globals.render_targets_disabled || !rasterizer_debug_options.draw_water )
        return;

    /* DEVIATION: the decompiler modeled the tag as stacked 0x28 `shader` layers (shader[N].base...);
     * retyped to the real shader_transparent_water tag (DB types_members-confirmed layout). */
    const shader_transparent_water *water = (const shader_transparent_water *)group->shader;
    int16_t primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);
    unsigned int vertex_declaration_index = primary_vertex_type;

    int16_t shader_variant;
    if ( primary_vertex_type == _vsdecl_environment || primary_vertex_type == _vsdecl_environment_lightmap )
        shader_variant = 0;
    else if ( primary_vertex_type == _vsdecl_model )
        shader_variant = 1;
    else
        shader_variant = 0;   /* dead default; water vertex types are only 0/2/4 */

    int suppresses_reflection = water->water.flags & (1u << _shader_transparent_water_draw_before_fog_bit);

    if ( !suppresses_reflection || (group->geometry_flags & (1u << _rasterizer_geometry_no_queue_bit)) != 0 || (group->geometry_flags & (1u << _rasterizer_geometry_sky_bit)) != 0 )
    {
        /* Depth-write only when this is not a decal-over-water and reflection is not suppressed. */
        unsigned char z_write = ((group->geometry_flags & (1u << _rasterizer_geometry_sky_bit)) == 0 && suppresses_reflection == 0) ? 1 : 0;

        if ( water_needs_update_flag )
        {
            rasterizer_water_build_bumpmap(group->shader);
            water_needs_update_flag = 0;
        }

        rasterizer_dx9_shader *reflection_shader = rasterizer_shader_select(_dxshader_transparent_water_opacity);
        if ( reflection_shader && reflection_shader->effect )
        {
            /* c0..c1: bump scale (x4) and reflection bumpiness (x4). */
            float pixel_constants[8];
            pixel_constants[0] = water->water.view_perpendicular_tint_color.alpha;
            pixel_constants[1] = water->water.view_perpendicular_tint_color.alpha;
            pixel_constants[2] = water->water.view_perpendicular_tint_color.alpha;
            pixel_constants[3] = water->water.view_perpendicular_tint_color.alpha;
            pixel_constants[4] = water->water.view_parallel_tint_color.alpha;
            pixel_constants[5] = water->water.view_parallel_tint_color.alpha;
            pixel_constants[6] = water->water.view_parallel_tint_color.alpha;
            pixel_constants[7] = water->water.view_parallel_tint_color.alpha;

            D3DDevice_SetVertexDeclaration(global_d3d_device,
                    rasterizer_dx9_shaders_vdecl9_get(vertex_declaration_index));
            D3DDevice_SetVertexShader(global_d3d_device,
                    rasterizer_dx9_shaders_vshader9_get(shader_variant + _vs_transparent_water_opacity));

            unsigned int pass_count[4];

            /* Pass 0 — reflection sampled through the normalization cube map. */
            if ( (water->water.flags & (1u << _shader_transparent_water_base_map_alpha_modulates_reflection_bit)) != 0 )
            {
                D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
                D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 8);
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
                D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
                D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
                D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, z_write);
                rasterizer_set_texture_for_effect(0, 0, 1, water->water.base_map.index,
                        group->shader_permutation_index, reflection_shader);
                water_clamp_sampler(0);
                rasterizer_set_texture_direct_for_effect(1, global_rasterizer_data->vector_normalization.index,
                        0, reflection_shader);
                water_clamp_sampler_w(1);
                ID3DXEffect_Begin(reflection_shader->effect, pass_count, 3);
                ID3DXEffect_BeginPass(reflection_shader->effect, 0);
                D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 2, (uint64_t)1 << 63);
                rasterizer_transparent_geometry_group_draw_internal(group, 0);
                ID3DXEffect_EndPass(reflection_shader->effect);
                ID3DXEffect_End(reflection_shader->effect);
            }

            /* Pass 1 — refraction, blended against what is already in the frame buffer. */
            if ( (water->water.flags & (1u << _shader_transparent_water_base_map_color_modulates_background_bit)) != 0 )
            {
                rasterizer_set_texture_for_effect(0, 0, 1, water->water.base_map.index,
                        group->shader_permutation_index, reflection_shader);
                water_clamp_sampler(0);
                D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
                D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
                D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 0);
                D3DDevice_SetRenderState_DestBlend(global_d3d_device, 4);
                D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
                D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
                D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
                D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
                D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, z_write);
                ID3DXEffect_Begin(reflection_shader->effect, pass_count, 3);
                ID3DXEffect_BeginPass(reflection_shader->effect, 1);
                D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 2, (uint64_t)1 << 63);
                rasterizer_transparent_geometry_group_draw_internal(group, 0);
                ID3DXEffect_EndPass(reflection_shader->effect);
                ID3DXEffect_End(reflection_shader->effect);
            }
        }

        /* Composited water surface. */
        rasterizer_dx9_shader *surface_shader = rasterizer_shader_select(_dxshader_transparent_water_reflection);
        if ( surface_shader && surface_shader->effect )
        {
            D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
            D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
            D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, (~group->geometry_flags >> 4) & 1);
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device,
                    (water->water.flags & (1u << _shader_transparent_water_base_map_alpha_modulates_reflection_bit)) == 0 ? 1 : 10);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
            D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
            D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
            D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
            D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, z_write);
            D3DDevice_SetVertexDeclaration(global_d3d_device,
                    rasterizer_dx9_shaders_vdecl9_get(vertex_declaration_index));
            D3DDevice_SetVertexShader(global_d3d_device,
                    rasterizer_dx9_shaders_vshader9_get(shader_variant + _vs_transparent_water_reflection));

            /* c10..c12: animated UV scroll for the surface, driven by frame game time. */
            float scroll_angle = water->water.ripple_animation_angle;
            float scroll_cos = (float)cos(scroll_angle);
            float scroll_sin = (float)sin(scroll_angle);
            float scroll_speed = water->water.ripple_animation_velocity;
            double game_time = global_frame_parameters.game_time_sec;
            float surface_constants[20];
            surface_constants[0]  = water->water.ripple_scale;
            surface_constants[1]  = water->water.ripple_scale;
            surface_constants[2]  = (float)(scroll_speed * scroll_cos * game_time);
            surface_constants[3]  = (float)((scroll_speed * scroll_sin) * game_time);
            surface_constants[4]  = 0.0f;
            surface_constants[5]  = 0.0f;
            surface_constants[6]  = 0.0f;
            surface_constants[7]  = 0.0f;
            surface_constants[8]  = 0.0f;
            surface_constants[9]  = 0.0f;
            surface_constants[10] = 0.0f;
            surface_constants[11] = 0.0f;
            D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, surface_constants, 3,
                    (uint64_t)3 << 60);

            int16_t reflection_mipmap = water->water.ripple_mipmap_levels;
            if ( reflection_mipmap > 1 )
                reflection_mipmap = 1;
            rasterizer_set_target_as_texture_for_effect(0, 8, reflection_mipmap, surface_shader);
            /* sampler 0: wrap-address reflection target. */
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 0);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
            rasterizer_set_texture_for_effect(3, 2, 0, water->water.reflection_map.index,
                    group->shader_permutation_index, surface_shader);
            water_clamp_sampler_w(3);

            /* Reflection tint: fade between the shader's two tint colors by how edge-on the water plane is. */
            float tint_red, tint_green, tint_blue;
            if ( __fsqrts(group->plane.n.n[2] * group->plane.n.n[2]
                        + (group->plane.n.n[0] * group->plane.n.n[0]
                         + group->plane.n.n[1] * group->plane.n.n[1])) <= 0.0f )
            {
                tint_red = 1.0f;
                tint_green = 1.0f;
                tint_blue = 1.0f;
            }
            else
            {
                float edge_on = -(group->plane.n.n[2] * global_window_parameters.camera.forward.n[2]
                        + (group->plane.n.n[0] * global_window_parameters.camera.forward.n[0]
                         + group->plane.n.n[1] * global_window_parameters.camera.forward.n[1]));
                if ( edge_on < 0.0f )
                    edge_on = 0.0f;
                else if ( edge_on > 1.0f )
                    edge_on = 1.0f;
                tint_red = water->water.view_parallel_tint_color.rgb.n[0] * (1.0f - edge_on)
                        + water->water.view_perpendicular_tint_color.rgb.n[0] * edge_on;
                tint_green = water->water.view_parallel_tint_color.rgb.n[1] * (1.0f - edge_on)
                        + water->water.view_perpendicular_tint_color.rgb.n[1] * edge_on;
                tint_blue = water->water.view_parallel_tint_color.rgb.n[2] * (1.0f - edge_on)
                        + water->water.view_perpendicular_tint_color.rgb.n[2] * edge_on;
            }

            unsigned int pass_count[4];
            ID3DXEffect_Begin(surface_shader->effect, pass_count, 3);
            for ( unsigned int pass = 0; pass < pass_count[0]; ++pass )
            {
                ID3DXEffect_BeginPass(surface_shader->effect, pass);
                /* pixel-shader constant c256 = per-pass reflection tint (inline Alu[256] poke). */
                float reflection_tint[4] = { tint_red, tint_green, tint_blue, 0.0f };
                D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 256, reflection_tint, 1, (uint64_t)1 << 63);
                rasterizer_transparent_geometry_group_draw_internal(group, 0);
                ID3DXEffect_EndPass(surface_shader->effect);
            }
            ID3DXEffect_End(surface_shader->effect);
        }
    }
    else
    {
        /* Unsupported vertex type — plain fixed-function textured pass. */
        D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);
        unsigned char z_write = 1; /* fallback pass always depth-writes */
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, z_write);
        D3DDevice_SetVertexDeclaration(global_d3d_device,
                rasterizer_dx9_shaders_vdecl9_get(vertex_declaration_index));
        D3DDevice_SetVertexShader(global_d3d_device,
                rasterizer_dx9_shaders_vshader9_get(shader_variant + _vs_transparent_water_opacity));
        D3DDevice_SetPixelShader(global_d3d_device, 0);
        SetTextureStageStateSmart(0, D3DTSS_COLOROP, 2);
        SetTextureStageStateSmart(0, D3DTSS_COLORARG1, 3);
        SetTextureStageStateSmart(0, D3DTSS_ALPHAOP, 2);
        SetTextureStageStateSmart(0, D3DTSS_ALPHAARG1, 3);
        SetTextureStageStateSmart(1, D3DTSS_COLOROP, 1);
        SetTextureStageStateSmart(1, D3DTSS_ALPHAOP, 1);
        rasterizer_transparent_geometry_group_draw_internal(group, 0);
    }
}
