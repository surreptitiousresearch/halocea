/* rasterizer_glass_draw_reflection_pp @0x8378DBC8 — draws the reflection pass of a shader_transparent_glass
 * geometry group. Picks one of three reflection effect shaders (106/107/108) by the reflection mode, uploads
 * the camera-forward direction and the glass tint / perpendicular-brightness constants to that effect, binds
 * the bump map (stage 0), the normalization cube map (stages 1/2) and either the current render target or a
 * bumped-reflection map (stage 3), programs each stage's sampler state, sets additive blend, and draws the
 * dynamic geometry once.
 *
 * Reconstructed from disassembly (0x8378DBC8-0x8378E400); the decompiler printed "local variable allocation
 * has failed". Deviations, all verified per-instruction:
 *   - `reflection_type` is DB-typed `_BOOL2` but is really a 0/1/2 reflection MODE (disasm 0x8378DDA8-DE0C:
 *     `cmplwi r25,1 / beq(==1) / cmplwi r25,3 / bge(skip)`, a 3-way branch, not a boolean). Renamed
 *     `reflection_mode` and typed `int`.
 *   - The four effect SetVector uploads are garbled CTR calls sharing one tail bctrl (LABEL_44). Traced from
 *     disasm 0x8378DE18-DF74: mode 0 uploads c0..c3 (guarded by constants!=null), mode 1 uploads c0..c2
 *     (NO null guard — the only branch that omits it), mode 2 uploads c0..c2 (guarded). c0=camera forward
 *     remapped to [-1,1] with w=1; c1/c2 = glass tint blocks; c3 = per-channel reflection intensity.
 *   - D3DDevice_SetVertexShaderConstantFN(0xA, ...) real trailing args are Vector4fCount=3, PendingMask0=3<<60
 *     (disasm 0x8378E024-E02C `li r6,3 / li r7,3 / sldi r7,r7,60`); the decompiler's 0x30000000 / &g_PFP[208]
 *     are GPR-shadow garble.
 *   - The AddressU/V/W / MagFilter / MinFilter / SeparateZFilterEnable pokes decompiled as raw m_Constants.Fetch
 *     / m_Pending.m_Mask writes are the compiler-inlined named sampler accessors (D3DDevice is a boundary type;
 *     same convention as rasterizer_dx9_transparent_chicago_draw). The address-mode value is r30 (=1, per the
 *     decompiler's `__ROL4__(1,11)` and the `v12 = r30 = 1` pvtype path). Stage-3 differs by mode: mode 2 binds
 *     the render target, sets only AddressU/V and SeparateZFilterEnable=0; every other mode binds a texture and
 *     sets AddressU/V/W and SeparateZFilterEnable=1 (disasm 0x8378E230-E36C).
 *   - Several texture-bind / MagFilter args the decompiler rendered as HIDWORD/DWORD1 shadows are the effect
 *     shader / constant 1 (verified: 0x8378E090 r8=r29, 0x8378E0CC r5=1, 0x8378E240 r5=1, 0x8378E2D4 r5=1).
 *   - shader-tag fields: the decompiler's shader[N].base.radiosity.* byte-offset puns (40-byte [N] stride)
 *     are resolved to the shader_transparent_glass DB members (reflection colours, bump/reflection map
 *     indices + scale, glass flags).
 * FAITHFUL QUIRK: for a primary vertex type other than 0/2/4 the vertex-shader sub-index is read from the
 * still-uninitialized effect pass-count buffer (reproduced verbatim). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/shader_transparent_glass.h"
#include "headers/shader_transparent_glass_flags.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/point2d.h"
#include "headers/shader_transparent_glass_reflection_type.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/blam_data_globals.h"
#include "headers/render_model_effect_type.h"
#include "headers/rasterizer_dx9_shader_index.h"


#include "headers/point2d.h"
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
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressW_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int Value);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int Value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int Value);

void rasterizer_glass_draw_reflection_pp(const transparent_geometry_group *group, int16_t reflection_mode)
{
    const shader_transparent_glass *glass = (const shader_transparent_glass *)group->shader;
    int16_t primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);

    /* c0: camera forward remapped to [0,1] then to [-1,1] per axis, w = 1. */
    float camera_forward[4];
    float forward_x = -(global_window_parameters.camera.forward.n[0] * 0.5f - 0.5f);
    forward_x = forward_x < 0.0f ? 0.0f : (forward_x > 1.0f ? 1.0f : forward_x);
    camera_forward[0] = forward_x * 2.0f - 1.0f;
    float forward_y = -(global_window_parameters.camera.forward.n[1] * 0.5f - 0.5f);
    forward_y = forward_y < 0.0f ? 0.0f : (forward_y > 1.0f ? 1.0f : forward_y);
    camera_forward[1] = forward_y * 2.0f - 1.0f;
    float forward_z = -(global_window_parameters.camera.forward.n[2] * 0.5f - 0.5f);
    forward_z = forward_z < 0.0f ? 0.0f : (forward_z > 1.0f ? 1.0f : forward_z);
    camera_forward[2] = forward_z * 2.0f - 1.0f;
    camera_forward[3] = 1.0f;

    /* c1/c2: reflection view-perpendicular / view-parallel colour blocks (r,g,b,a). */
    float tint_constants[4];
    tint_constants[0] = glass->glass.reflection_view_perpendicular_color.__s2.red;
    tint_constants[1] = glass->glass.reflection_view_perpendicular_color.__s2.green;
    tint_constants[2] = glass->glass.reflection_view_perpendicular_color.__s2.blue;
    tint_constants[3] = glass->glass.reflection_view_perpendicular_color.__s1.alpha;

    float reflection_constants[4];
    reflection_constants[0] = glass->glass.reflection_view_parallel_color.__s2.red;
    reflection_constants[1] = glass->glass.reflection_view_parallel_color.__s2.green;
    reflection_constants[2] = glass->glass.reflection_view_parallel_color.__s2.blue;
    reflection_constants[3] = glass->glass.reflection_view_parallel_color.__s1.alpha;

    /* c3: reflection intensity replicated to all four channels. */
    float reflection_intensity = (group->effect.type == _render_model_effect_type_active_camouflage) ? 1.0f - group->effect.intensity : 1.0f;
    float intensity_constants[4];
    intensity_constants[0] = reflection_intensity;
    intensity_constants[1] = reflection_intensity;
    intensity_constants[2] = reflection_intensity;
    intensity_constants[3] = reflection_intensity;

    /* Promote mode 0 to 1 when this glass surface forces the flat-reflection path. */
    if (!reflection_mode)
        reflection_mode = (glass->glass.flags & (1u << _shader_transparent_glass_bump_map_is_specular_mask_bit)) != 0;
    if (!reflection_mode)
        reflection_mode = glass->glass.reflection_bump_map.index == -1;

    /* vertex-shader sub-index by primary vertex type; the else path reads the uninitialized pass-count buffer. */
    unsigned int effect_pass_count[2]; /* Begin() out-param */
    int16_t vshader_subindex;
    if (primary_vertex_type == _rasterizer_vertex_type_environment_uncompressed
        || primary_vertex_type == _rasterizer_vertex_type_environment_lightmap_uncompressed)
        vshader_subindex = 0;
    else if (primary_vertex_type == _rasterizer_vertex_type_model_uncompressed)
        vshader_subindex = 1;
    else
        vshader_subindex = (int16_t)effect_pass_count[0]; /* FAITHFUL QUIRK: uninitialized read */

    /* the bumped-mode shader is selected unconditionally; modes 1/2 replace it. */
    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_transparent_glass_reflection_bumped);
    int16_t vshader_base = 0;
    if (reflection_mode == _shader_transparent_glass_reflection_type_bumped)
    {
        vshader_base = 50;
        if (effect_shader->constants)
        {
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[0],
                                             (const D3DXVECTOR4 *)camera_forward);
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[1],
                                             (const D3DXVECTOR4 *)tint_constants);
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[2],
                                             (const D3DXVECTOR4 *)reflection_constants);
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[3],
                                             (const D3DXVECTOR4 *)intensity_constants);
        }
    }
    else if (reflection_mode == _shader_transparent_glass_reflection_type_flat)
    {
        effect_shader = rasterizer_shader_select(_dxshader_transparent_glass_reflection_flat);
        vshader_base = 52;
        /* DEVIATION: no constants!=null guard here — the disasm dereferences constants directly. */
        ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[0],
                                         (const D3DXVECTOR4 *)camera_forward);
        ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[1],
                                         (const D3DXVECTOR4 *)tint_constants);
        ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[2],
                                         (const D3DXVECTOR4 *)reflection_constants);
    }
    else if (reflection_mode == _shader_transparent_glass_reflection_type_mirror)
    {
        effect_shader = rasterizer_shader_select(_dxshader_transparent_glass_reflection_mirror);
        vshader_base = 54;
        if (effect_shader->constants)
        {
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[0],
                                             (const D3DXVECTOR4 *)camera_forward);
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[1],
                                             (const D3DXVECTOR4 *)tint_constants);
            ID3DXEffect_SetVector(effect_shader->effect, effect_shader->constants[2],
                                             (const D3DXVECTOR4 *)reflection_constants);
        }
    }
    /* reflection_mode >= 3: no effect uploads; keeps the mode-0 shader and vshader_base 0. */

    int reflection_pass = (glass->glass.flags >> _shader_transparent_glass_bump_map_is_specular_mask_bit) & 1;
    D3DDevice_SetVertexDeclaration(global_d3d_device,
                                   rasterizer_dx9_shaders_vdecl9_get((unsigned int)primary_vertex_type));
    D3DDevice_SetVertexShader(global_d3d_device,
                              rasterizer_dx9_shaders_vshader9_get(vshader_subindex + vshader_base));

    if (effect_shader && effect_shader->effect)
    {
        /* Vertex-shader constants c10..c12: projection scale + screen half-extents. */
        float vs_constants[12];
        vs_constants[0] = group->model_base_map_scale.n[0] * glass->glass.reflection_bump_map_scale;
        vs_constants[1] = group->model_base_map_scale.n[1] * glass->glass.reflection_bump_map_scale;
        vs_constants[2] = (float)(global_window_parameters.camera.viewport_bounds.__s1.x1
                                  - global_window_parameters.camera.viewport_bounds.__s1.x0) * 0.5f; /* width/2 */
        vs_constants[3] = (float)(global_window_parameters.camera.viewport_bounds.__s1.y1
                                  - global_window_parameters.camera.viewport_bounds.__s1.y0) * 0.5f; /* height/2 */
        vs_constants[4] = 0.0f;
        vs_constants[5] = 0.0f;
        vs_constants[6] = 0.0f;
        vs_constants[7] = 0.0f;
        vs_constants[8] = 0.0f;
        vs_constants[9] = 1.0f;
        vs_constants[10] = 0.0f;
        vs_constants[11] = 0.0f;
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, vs_constants, 3, (uint64_t)3 << 60);

        /* stage 0: bump map. */
        rasterizer_set_texture_for_effect(0, 0, 3, glass->glass.reflection_bump_map.index,
                                          group->shader_permutation_index, effect_shader);

        /* stage 1: normalization cube map. */
        rasterizer_set_texture_direct_for_effect(1, global_rasterizer_data->vector_normalization.index, 0,
                                                 effect_shader);
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 0);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 1, 0);

        /* stage 2: normalization cube map (second sample). */
        rasterizer_set_texture_direct_for_effect(2, global_rasterizer_data->vector_normalization.index, 0,
                                                 effect_shader);
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, 1);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, 1);
        D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 2, 1);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, 0);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 2, 0);

        /* stage 3: render target (mirror mode) or reflection texture (all other modes). */
        if (reflection_mode == _shader_transparent_glass_reflection_type_mirror)
        {
            rasterizer_set_target_as_texture_for_effect(3, 2, 0, effect_shader);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 3, 0);
        }
        else
        {
            rasterizer_set_texture_for_effect(3, 2, 0, glass->glass.reflection_map.index,
                                              group->shader_permutation_index, effect_shader);
            D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_AddressW_Inline(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 3, 1);
            D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 3, 1);
        }

        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 6);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);

        ID3DXEffect_Begin(effect_shader->effect, effect_pass_count, 3);
        ID3DXEffect_BeginPass(effect_shader->effect, reflection_pass);
        rasterizer_transparent_geometry_group_draw_internal(group, 0);
        ID3DXEffect_EndPass(effect_shader->effect);
        ID3DXEffect_End(effect_shader->effect);
    }
}
