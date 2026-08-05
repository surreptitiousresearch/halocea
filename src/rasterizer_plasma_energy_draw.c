/* rasterizer_plasma_energy_draw @0x8382DD30 — additive glow pass for the "plasma energy" transparent-shader
 * effect (shader index 44): computes an animated glow-color scale and, separately, an animated tint-blue
 * scale from the shader's animation-value table (each raised to a per-shader exponent via pow()), uploads
 * both a per-vertex glow-color constant block (c13..c18, four repeated RGB triples plus alpha-time terms)
 * and a texture-transform constant block (c10..c12, base-map vs. detail-map UV scroll/scale derived from the
 * animation-selected color and the two pow() results), binds the base map (stage 0) and a second map
 * (stage 1) through the effect, forces wrap-U/wrap-W addressing and point filtering with Z-sort-friendly
 * additive blending, and draws the group through the shader's D3DX effect (one BeginPass/EndPass per pass).
 * Runs only when rasterizer_debug_options.plasma_energy_enabled.
 *
 * DEVIATIONS (all disasm-resolved; the decompiler's own local-variable-allocation warning on this function
 * was a real red flag — every long-double/__int128/OVERLAPPED pun below was decompiler noise, not real
 * data flow):
 *  - The decompiler declared 3 extra float/double parameters (a2, a3, a4) beyond `group` and rendered both
 *    pow() calls as `pow(v13, a4)` / `pow(v16, a4)` with a punned "long double" first argument straddling two
 *    stores — but the DB's own registered prototype (`funcs.prototype`) takes only `group`, and neither a4
 *    nor any other phantom parameter reaches either pow() call in the disasm (0x8382DDFC, 0x8382DE34): the
 *    real calls are `pow(values[intensity_source-1], plasma.intensity_exponent)` and
 *    `pow(group->animation->values[offset_source-1], plasma.offset_exponent)`, f1/f2 loaded
 *    directly with no punning and no reference to a2/a3/a4 anywhere in the function.
 *  - Both rasterizer_set_texture_for_effect calls: the decompiler's `usage` argument for the second call
 *    (rendered `SWORD1(v28)`, a value clawed out of an unrelated m_Pending.m_Mask store) is simply 0
 *    (disasm 0x8382DF78 `li r5,0`) — identical to the first call's usage.
 *  - The four `m_Constants.Fetch[stage].Texture.dword[...]` / `m_Pending.m_Mask[3]` raw pokes per stage
 *    (0-1) are the compiler-inlined D3DDevice_SetSamplerState_{AddressW,AddressU,SeparateZFilterEnable}
 *    helpers, identified by hand-tracing each rlwimi/rlwinm/insrwi bit-field against the real (still-named)
 *    inline clones of those three setters elsewhere in the binary: AddressW_Inline clears/sets bits 16-18
 *    (mask 0x70000, dword[0]), AddressU_Inline bits 10-12 (mask 0x1C00, dword[0]), SeparateZFilterEnable
 *    bits 23-24 (dword[3]). NOTE: this contradicts a DEVIATION comment in the already-committed sibling
 *    rasterizer_active_camouflage_cache_primary_render_target.c, which labels the SAME bits 16-18/19-21
 *    pair as AddressV/AddressU — that file's naming was not re-verified against the actual named inline
 *    function bodies during that session and should be treated as suspect (this file's mapping was
 *    cross-checked directly against D3DDevice_SetSamplerState_AddressU_Inline / _AddressV_Inline /
 *    _AddressW_Inline's own decompiled bit masks: U=0x1C00, V=0xE000, W=0x70000).
 *  - D3DDevice_SetVertexShaderConstantFN's Vector4fCount/PendingMask0 args, rendered as garbled 32-bit
 *    fragments of a 64-bit pun (`0x18000000u`/`v18` and `0x30000000u`/`v41`), are actually the plain small
 *    integers 6 / ((unsigned __int64)3 << 59) and 3 / ((unsigned __int64)3 << 60) (disasm confirms
 *    `li r6,6`/`sldi r7,r7,59` and `li r6,3`/`sldi r7,r7,60` immediately before each call).
 *  - D3DDevice_SetRenderState_CullMode's argument (rendered `HIDWORD(v40)`, another pun fragment) is a plain
 *    0 (disasm 0x8382E0B0 `li r4,0`, unchanged through to the call at 0x8382E0D4).
 *  - The glow-color constant block reuses ONE raw (unmultiplied) float, the secondary noise map scale
 *    (plasma.secondary_noise_map_scale, loaded once at 0x8382DE78 as f13), at three separate output slots
 *    (c13.w/c15.z/c18.x in register terms; array indices 12/17/22 here) — the decompiler's
 *    `v48[12]`/`v48[17]`/`v48[22]` all showed the unrelated primary direction.y value instead, because both
 *    are named identically by field path once the decompiler collapses the register alias; disasm
 *    (0x8382DE78/0x8382DF00/0x8382DF18/0x8382DF30, all reading the same f13) is what disambiguates them. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader_transparent_plasma.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/global_frame_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"


#include "headers/point2d.h"
extern double pow(double x, double y);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
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
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);

void rasterizer_plasma_energy_draw(const transparent_geometry_group *group)
{
    if ( !rasterizer_debug_options.plasma_energy_enabled )
        return;

    const shader_transparent_plasma *plasma = (const shader_transparent_plasma *)group->shader;
    const render_animation *animation = group->animation;
    const real_rgb_color *animation_color = global_real_rgb_white;
    float glow_scale = 1.0f;         /* v7 */
    float tint_blue_scale = 0.0f;    /* v9 */

    if ( animation )
    {
        if ( animation->colors )
        {
            int16_t color_index = plasma->plasma.tint_color_source;
            if ( color_index >= 1 && color_index <= 4 )
                animation_color = &animation->colors[color_index - 1];
        }

        const float *values = animation->values;
        if ( values )
        {
            int16_t intensity_source = plasma->plasma.intensity_source;
            if ( intensity_source >= 1 && intensity_source <= 4 )
                glow_scale = (float)pow(values[intensity_source - 1], plasma->plasma.intensity_exponent);

            int16_t offset_source = plasma->plasma.offset_source;
            if ( offset_source >= 1 && offset_source <= 4 )
            {
                tint_blue_scale = (float)pow(group->animation->values[offset_source - 1],
                        plasma->plasma.offset_exponent) * plasma->plasma.offset_amount;
                if ( tint_blue_scale < 0.00050000002 )
                    tint_blue_scale = 0.0f;
            }
        }
    }

    rasterizer_dx9_shader *dxeffect_shader = rasterizer_shader_select(_dxshader_transparent_plasma);
    if ( !dxeffect_shader || !dxeffect_shader->effect )
        return;

    float primary_dir_x = plasma->plasma.primary_noise_map_animation_direction.n[0];
    float primary_dir_y = plasma->plasma.primary_noise_map_animation_direction.n[1];
    float secondary_map_scale = plasma->plasma.secondary_noise_map_scale;   /* raw, unscaled — reused 3x below */

    float time_over_period_1 = (float)(global_frame_parameters.game_time_sec
            / plasma->plasma.secondary_noise_map_animation_period);
    float time_over_period_2 = (float)(global_frame_parameters.game_time_sec
            / plasma->plasma.primary_noise_map_animation_period);

    /* c13..c18: four repeated RGB triples (glow color / 0 / tint-blue) plus per-triple alpha-time terms */
    float glow_color_constants[24];
    glow_color_constants[0]  = plasma->plasma.primary_noise_map_scale;
    glow_color_constants[1]  = 0.0f;
    glow_color_constants[2]  = tint_blue_scale;
    glow_color_constants[3]  = primary_dir_x * time_over_period_2;
    glow_color_constants[4]  = 0.0f;
    glow_color_constants[5]  = plasma->plasma.primary_noise_map_scale;
    glow_color_constants[6]  = 0.0f;
    glow_color_constants[7]  = primary_dir_y * time_over_period_2;
    glow_color_constants[8]  = 0.0f;
    glow_color_constants[9]  = 0.0f;
    glow_color_constants[10] = plasma->plasma.primary_noise_map_scale;
    glow_color_constants[11] = plasma->plasma.primary_noise_map_animation_direction.n[2] * time_over_period_2;
    glow_color_constants[12] = secondary_map_scale;
    glow_color_constants[13] = 0.0f;
    glow_color_constants[14] = 0.0f;
    glow_color_constants[15] = (plasma->plasma.secondary_noise_map_animation_direction.n[0] * time_over_period_1);
    glow_color_constants[16] = 0.0f;
    glow_color_constants[17] = secondary_map_scale;
    glow_color_constants[18] = 0.0f;
    glow_color_constants[19] = (plasma->plasma.secondary_noise_map_animation_direction.n[1] * time_over_period_1);
    glow_color_constants[20] = 0.0f;
    glow_color_constants[21] = 0.0f;
    glow_color_constants[22] = secondary_map_scale;
    glow_color_constants[23] = plasma->plasma.secondary_noise_map_animation_direction.n[2] * time_over_period_1;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, glow_color_constants, 6,
            (uint64_t)3 << 59);

    rasterizer_set_texture_for_effect(0, 1, 0, plasma->plasma.primary_noise_map.index,
            group->shader_permutation_index, dxeffect_shader);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 0, 0);
    rasterizer_set_texture_for_effect(1, 1, 0, plasma->plasma.secondary_noise_map.index,
            group->shader_permutation_index, dxeffect_shader);
    D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 1, 0);

    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);

    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 1);

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 6);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, 3);

    /* c10..c12: base-map / detail-map UV scale-bias, x/y pairs scaled by the animation color and glow_scale */
    float texture_transform_constants[12];
    texture_transform_constants[0]  = 1.0f;
    texture_transform_constants[1]  = 1.0f;
    texture_transform_constants[2]  = 1.0f;
    texture_transform_constants[3]  = 1.0f;
    texture_transform_constants[4]  = (plasma->plasma.view_perpendicular_color.__s2.red
            - plasma->plasma.view_parallel_color.__s2.red) * animation_color->n[0];
    texture_transform_constants[5]  = (plasma->plasma.view_perpendicular_color.__s2.green
            - plasma->plasma.view_parallel_color.__s2.green) * animation_color->n[1];
    texture_transform_constants[6]  = (plasma->plasma.view_perpendicular_color.__s2.blue
            - plasma->plasma.view_parallel_color.__s2.blue) * animation_color->n[2];
    texture_transform_constants[7]  = (plasma->plasma.view_perpendicular_color.__s1.alpha
            - plasma->plasma.view_parallel_color.__s1.alpha) * glow_scale;
    texture_transform_constants[8]  = plasma->plasma.view_parallel_color.__s2.red * animation_color->n[0];
    texture_transform_constants[9]  = plasma->plasma.view_parallel_color.__s2.green * animation_color->n[1];
    texture_transform_constants[10] = plasma->plasma.view_parallel_color.__s2.blue * animation_color->n[2];
    texture_transform_constants[11] = plasma->plasma.view_parallel_color.__s1.alpha * glow_scale;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, texture_transform_constants, 3,
            (uint64_t)3 << 60);

    unsigned int pass_count[4];
    ID3DXEffect_Begin(dxeffect_shader->effect, pass_count, 3);
    int16_t primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(primary_vertex_type));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_transparent_plasma_m));

    for ( unsigned int pass = 0; pass < pass_count[0]; ++pass )
    {
        ID3DXEffect_BeginPass(dxeffect_shader->effect, pass);
        rasterizer_transparent_geometry_group_draw_internal(group, 0);
        ID3DXEffect_EndPass(dxeffect_shader->effect);
    }
    ID3DXEffect_End(dxeffect_shader->effect);
}
