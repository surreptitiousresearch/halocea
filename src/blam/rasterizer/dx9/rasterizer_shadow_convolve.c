/* rasterizer_shadow_convolve @0x837A31F0 — post-process pass that convolves the raw shadow-map render
 * target (target 3) into the blurred shadow buffer, using effect 45. Binds render target 3 as texture
 * inputs on samplers 0-3 (mirror addressing, linear min/mag filtering, separate-Z filtering), retargets
 * output to target 4, and draws a full-viewport quad through a fixed 8x1 pixel-offset sample-tap matrix
 * (technique pass 0), then draws a 128x128 box outline as an 8-vertex line list (technique pass 1)
 * showing the convolution kernel's sample footprint, nudged by the global `ofsx`/`ofsy` screen-shake
 * offsets. The line-list pass has NO guard of its own — both draws are gated only by the
 * `shadow_convolution_enabled` check at entry (@0x837A322C); the disassembly holds no branch between
 * 0x837A337C and the second draw at 0x837A37B4.
 *
 * DEVIATION ("local variable allocation failed"): the per-stage sampler setup (address-mode U/V, min/mag
 * filter, separate-Z filter) was emitted as raw inline GPU pending-register pokes
 * (`global_d3d_device->m_Pending.m_Mask[...]` with `__ROL4__`/`__ROR4__` bit-rotate masks); restored to
 * the equivalent `D3DDevice_SetSamplerState_*_Inline` calls already established for this exact pattern in
 * sibling rasterizer begin/draw functions (`_rasterizer_decals_begin.c`,
 * `_rasterizer_environment_diffuse_textures_begin.c`) — same call set, same value 1, which on the 360
 * numbering is D3DTADDRESS_MIRROR / D3DTEXF_LINEAR / enabled (address poke rlwimi r10,r27,11,19,21
 * @0x837A32B0 and rlwimi r3,r27,14,16,18 @0x837A32D0, filter li r5,1 @0x837A32EC, r27 = 1).
 *
 * DEVIATION: both `D3DDevice_SetVertexShaderConstantFN` calls show corrupted `Vector4fCount`/`PendingMask0`
 * arguments (`0x1C000000u`/a string-literal cast, and an unassigned local, respectively); both resolved via
 * disasm to the real register values: first call `Vector4fCount=8, PendingMask0=(uint64)7<<58` (matching
 * the 8-vector4 sample-tap matrix `v19`), second call `Vector4fCount=5, PendingMask0=(uint64)3<<59` (the
 * same encoding used throughout this codebase's other 5-vector4 uploads).
 *
 * DEVIATION: the decompile shows the debug-grid line-list vertices as ~30 individually-named globals
 * (the `shadow_grid_vertices` block through its trailing texcoord.y field @+0x14 of record 7) with no
 * aggregate symbol in the database; they occupy one contiguous
 * 192-byte block (8 `dynamic_screen_vertex` records, matching the `D3DPT_LINELIST`/8-vertex/0x18-stride
 * draw call) — reconstructed here as `shadow_grid_vertices[8]`, a file-static array (color/z/texcoord are
 * always 0 in every record; only position.xy varies, tracing out the four edges of a 128x128 box). */

#include <stdint.h>
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int stride);

/* Debug-grid outline for the convolution kernel footprint: 4 independent lines forming a 128x128 box. */
static dynamic_screen_vertex shadow_grid_vertices[8] = {
    { { -1.0f, 0.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { 128.0f, 0.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { 127.0f, -1.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { 127.0f, 128.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { 128.0f, 127.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { -1.0f, 127.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { 0.0f, 128.0f, 0.0f }, 0, { 0.0f, 0.0f } },
    { { 0.0f, -1.0f, 0.0f }, 0, { 0.0f, 0.0f } },
};

void rasterizer_shadow_convolve(void)
{
    if (rasterizer_globals.render_targets_disabled)
        return;
    if (!rasterizer_debug_options.draw_environment_shadows)
        return;
    if (!rasterizer_debug_options.shadow_convolution_enabled)
        return;

    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_shadow_convolve);
    if (!shader || !shader->effect)
        return;

    for (int16_t stage = 0; stage < 4; stage++)
    {
        rasterizer_set_target_as_texture_for_effect(stage, 3, 0, shader);
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, stage, D3DTADDRESS_MIRROR);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, stage, D3DTADDRESS_MIRROR);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, stage, 1);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, stage, D3DTEXF_LINEAR);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, stage, D3DTEXF_LINEAR);
    }

    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    /* 8-vector4 sample-tap matrix: 4 diagonal +/-1/256px offset pairs for a 4-tap box blur. */
    float sample_taps[32] = {
        1.0f, 0.0f, 0.0f, -0.00390625f,
        0.0f, 1.0f, 0.0f, -0.00390625f,
        1.0f, 0.0f, 0.0f, 0.00390625f,
        0.0f, 1.0f, 0.0f, 0.00390625f,
        1.0f, 0.0f, 0.0f, -0.00390625f,
        0.0f, 1.0f, 0.0f, 0.00390625f,
        1.0f, 0.0f, 0.0f, 0.00390625f,
        0.0f, 1.0f, 0.0f, -0.00390625f,
    };
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, sample_taps, 8, (uint64_t)7 << 58);

    rasterizer_set_target(4, 0, 0, 0, 0);

    vertex_data_2[0].position = (real_point3d){ { -1.0078125f, 1.0078125f, 0.0f } };
    vertex_data_2[0].color = 0;
    vertex_data_2[0].texcoord = (real_point2d){ { 0.0f, 0.0f } };
    vertex_data_2[1].position = (real_point3d){ { 0.9921875f, 1.0078125f, 0.0f } };
    vertex_data_2[1].color = 0;
    vertex_data_2[1].texcoord = (real_point2d){ { 1.0f, 0.0f } };
    vertex_data_2[2].position = (real_point3d){ { 0.9921875f, -0.9921875f, 0.0f } };
    vertex_data_2[2].color = 0;
    vertex_data_2[2].texcoord = (real_point2d){ { 1.0f, 1.0f } };
    vertex_data_2[3].position = (real_point3d){ { -1.0078125f, -0.9921875f, 0.0f } };
    vertex_data_2[3].color = 0;
    vertex_data_2[3].texcoord = (real_point2d){ { 0.0f, 1.0f } };

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_convolution));

    unsigned int pass_count[4];
    ID3DXEffect_Begin(shader->effect, pass_count, 3);
    ID3DXEffect_BeginPass(shader->effect, 0);
    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_2, sizeof(dynamic_screen_vertex));
    ID3DXEffect_EndPass(shader->effect);
    ID3DXEffect_End(shader->effect);

    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));

    /* Viewport-to-NDC transform (5 vector4s), nudged by the screen-shake offsets. */
    float grid_transform[20] = {
        0.015625f, 0.0f,        0.0f, ofsx,
        0.0f,      -0.015625f,  0.0f, ofsy,
        0.0f,      0.0f,        0.0f, 0.5f,
        0.0f,      0.0f,        0.0f, 1.0f,
        1.0f,      1.0f,        0.0f, 0.0f,
    };
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, grid_transform, 5, (uint64_t)3 << 59);

    ID3DXEffect_Begin(shader->effect, pass_count, 3);
    ID3DXEffect_BeginPass(shader->effect, 1);
    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_LINELIST, 8, shadow_grid_vertices,
            sizeof(dynamic_screen_vertex));
    ID3DXEffect_EndPass(shader->effect);
    ID3DXEffect_End(shader->effect);
}
