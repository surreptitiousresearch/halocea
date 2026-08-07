/* rasterizer_water_build_bumpmap @0x83787718 -- render the animated bump/normal map used by water shading:
 * a fullscreen quad blended additively through up to `layer_count` passes (one render-target mip per pass,
 * targets 0..layer_count-1 of render target 8), each pass rotating and scrolling a sub-layer's texture by
 * its own angle/amplitude/base-offset over game time. The per-pass pixel constant is a fixed
 * (0.502, 0.502, 1.0) base bump color (a neutral/flat tangent-space normal) plus three inter-layer blend
 * weights and a single varying alpha-role channel that ramps 0..tint_color.n[0] across the passes.
 * Runs only when rasterizer_debug_options.draw_water.
 *
 * Reconstructed from disassembly (decompiler reported "local variable allocation has failed"). The
 * decompiler knew only the 40-byte `shader` base, so it folded each concrete-tag byte offset into a
 * subscript on it (`shader[N].base.radiosity.*`, N = byte_offset / 40); all four reads are really members
 * of shader_transparent_water's own body, which begins at +0x28. Re-derived from the loads:
 *   0xD4 `lwz` @0x83787DAC -> water.ripple_maps.index          (folded shader[5].base.radiosity.color.green)
 *   0xD8 `lhz` @0x837878D8 -> water.ripple_mipmap_levels       (folded shader[5].base.radiosity.color.n[2])
 *   0xDC `lfs` @0x83787CBC -> water.ripple_mipmap_fade_factor  (folded shader[5].base.radiosity.tint_color.n[0])
 *   0x124/0x128 `lwz` @0x83787AB8 -> water.ripples.count/.address (folded shader[7].base.radiosity.color.*)
 * DEVIATIONS, all disasm-verified:
 * (1) the four per-stage `m_Constants.Fetch[stage].Texture.dword[...]`/`m_Pending.m_Mask[3]` raw pokes are
 * the compiler-inlined AddressU/AddressV/SeparateZFilterEnable setters (bit ranges cross-checked against
 * the already-verified sibling rasterizer_plasma_energy_draw.c: U=0x1C00, V=0xE000, dword[0]; the
 * dword[3] poke here sets bit 23 only, value 1). (2) D3DDevice_SetVertexShaderConstantFN's garbled
 * count/mask (rendered `0x1C000000u`) is really (device, 0xD, texture_transforms, 8, (uint64)7<<58) --
 * disasm 0x83787C24 `li r7,7`/`li r6,8`/`sldi r7,r7,58`. (3) D3DDevice_SetPixelShaderConstantFN's mask
 * (the decompiler's `HIDWORD(v61)=__ROR4__(1,1)`, a 32-bit-only fragment) is the plain 64-bit
 * (unsigned __int64)1<<63 -- disasm 0x83787CA4 `extldi r28,r8,64,63` with r8=1 is rotr64(1,63) == 1<<63,
 * not a 32-bit rotate. (4) the per-layer alpha-role ratio (decompiler's
 * `__SPAIR64__(tint_source_count-1, stage_index)` pun) is the plain
 * `(float)stage_index / (float)(tint_source_count-1)` -- disasm 0x83787CB8-0x83787CFC converts r27
 * (stage_index) and r11 (tint_source_count-1) to float independently and divides, no 64-bit pair involved.
 * (5) the decompiler's `v34`/`v35`/`v36` inter-layer blend inputs (fed through real_argb_color_to_pixel32
 * then unpacked back to floats via `v74`/`v75`/`v76`) are, by their stack strides, layers[0..2]'s own
 * rotation_amplitude field re-read (confirmed: v85/v87/v88/v89 sit exactly 76 bytes apart, matching
 * water_bumpmap_layer's size, at offset +4 within each entry) -- the SAME field the animation loop uses for
 * scroll speed, dual-purposed here as a blend weight; the decompiler's naming made these look like
 * unrelated fresh locals. The pack/unpack round trip that follows is provably a no-op for the RGB channels
 * (real_argb_color_to_pixel32's input always has rgb=0, and the result is then forced through `| 0x8080FF`
 * before being unpacked) -- red/green always resolve to 0x80/255 = 0.50196081f and blue to 0xFF/255 = 1.0f
 * regardless of the ratio, so the round trip is replaced here with the equivalent literal constants; only
 * the separately-computed alpha-role channel actually varies. (6) each layer's 32-byte texture-transform
 * constant pair actually starts 4 bytes before the decompiler's `v84` buffer (at its own `v83` local) -- the
 * true per-layer stride is 32 bytes: {uv_scale, 0, 0, u_offset}, {0, uv_scale, 0, v_offset}, confirmed by
 * the SetVertexShaderConstantFN upload spanning exactly 8 registers (128 bytes) starting at &v83. */

#include <stdint.h>
#include <string.h>
#include "headers/shader.h"
#include "headers/shader_transparent_water.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/global_frame_parameters.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"

#include <math.h>
#include "headers/blam_data_globals.h"

/* one entry of the water_bumpmap_layer tag block referenced by water.ripples (tag +0x124, the offset the
 * decompiler folded into shader[7]); field names/offsets derived from disasm usage
 * (0x83787B68-0x83787C1C), 76 bytes per entry */
typedef struct water_bumpmap_layer
{
    float   rotation_angle;     /* 0x00 */
    float   rotation_amplitude; /* 0x04 */
    float   u_base;             /* 0x08 */
    float   v_base;             /* 0x0C */
    char    unused[4];          /* 0x10 */
    int16_t uv_scale;           /* 0x14 */
    int16_t bitmap_index;       /* 0x16 */
    char    unused2[52];        /* 0x18 */
} water_bumpmap_layer; /* 76 bytes */



#include "headers/point2d.h"
/* memcpy declared by <string.h> */
/* cos provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* sin provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask1);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertices, unsigned int stride);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);

void rasterizer_water_build_bumpmap(const shader *shader)
{
    if ( !rasterizer_debug_options.draw_water )
        return;

    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_transparent_water_bumpmap_convolution);
    if ( !effect_shader || !effect_shader->effect )
        return;

    vertex_data_0[0].texcoord.n[0] = 0.0f;
    vertex_data_0[0].texcoord.n[1] = 0.0f;
    vertex_data_0[0].color = 0xFFFFFFFF;
    vertex_data_0[0].position.n[0] = -1.0078125f;
    vertex_data_0[0].position.n[1] = 1.0078125f;
    vertex_data_0[0].position.n[2] = 0.0f;
    vertex_data_0[1].texcoord.n[0] = 1.0f;
    vertex_data_0[1].texcoord.n[1] = 0.0f;
    vertex_data_0[1].color = 0xFFFFFFFF;
    vertex_data_0[1].position.n[0] = 0.9921875f;
    vertex_data_0[1].position.n[1] = 1.0078125f;
    vertex_data_0[1].position.n[2] = 0.0f;
    vertex_data_0[2].texcoord.n[0] = 1.0f;
    vertex_data_0[2].texcoord.n[1] = 1.0f;
    vertex_data_0[2].color = 0xFFFFFFFF;
    vertex_data_0[2].position.n[0] = 0.9921875f;
    vertex_data_0[2].position.n[1] = -0.9921875f;
    vertex_data_0[2].position.n[2] = 0.0f;
    vertex_data_0[3].texcoord.n[0] = 0.0f;
    vertex_data_0[3].texcoord.n[1] = 1.0f;
    vertex_data_0[3].color = 0xFFFFFFFF;
    vertex_data_0[3].position.n[0] = -1.0078125f;
    vertex_data_0[3].position.n[1] = -0.9921875f;
    vertex_data_0[3].position.n[2] = 0.0f;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    for ( unsigned int stage = 0; stage < 4; ++stage )
    {
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, stage, 0);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, stage, 0);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, stage, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, stage, 1);
        D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, stage, 1);
    }

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_convolution));

    /* DEVIATION: the decompiler modeled the tag as stacked 0x28 `shader` layers (shader[N].base...);
     * retyped to the real shader_transparent_water tag (DB types_members-confirmed layout). */
    const shader_transparent_water *water = (const shader_transparent_water *)shader;

    int16_t layer_count = 4;
    if ( water->water.ripple_mipmap_levels <= 4 )
        layer_count = water->water.ripple_mipmap_levels;

    /* per-layer texture-transform constants (c13..c20): {uv_scale,0,0,u_offset},{0,uv_scale,0,v_offset} */
    float texture_transforms[32];
    /* pixel shader constants: reg0..2 carry the three inter-layer blend ratios in their w component
     * (xyz unused), reg3 carries the base bump color (see the "provably constant" note below) */
    float pixel_constants[16];
    pixel_constants[0] = 0.0f;
    pixel_constants[1] = 0.0f;
    pixel_constants[2] = 0.0f;
    pixel_constants[4] = 0.0f;
    pixel_constants[5] = 0.0f;
    pixel_constants[6] = 0.0f;
    pixel_constants[8] = 0.0f;
    pixel_constants[9] = 0.0f;
    pixel_constants[10] = 0.0f;

    int reflection_count = water->water.ripples.count;
    water_bumpmap_layer *reflection_layers = (water_bumpmap_layer *)water->water.ripples.address;
    water_bumpmap_layer layers[4];
    for ( int16_t layer_index = 0; layer_index < 4; ++layer_index )
    {
        if ( layer_index >= reflection_count )
        {
            memset(&layers[layer_index], 0, sizeof(water_bumpmap_layer));
            layers[layer_index].uv_scale = 1;
        }
        else
        {
            memcpy(&layers[layer_index], &reflection_layers[layer_index], sizeof(water_bumpmap_layer));
        }
    }

    /* inter-layer blend weights: rotation_amplitude, dual-purposed here (per-entry stride confirms
     * layers[0..3].rotation_amplitude, not a separate field) */
    float blend_a = layers[0].rotation_amplitude;
    float blend_b = layers[1].rotation_amplitude;
    if ( layers[0].rotation_amplitude == 0.0f && blend_b == 0.0f )
    {
        layers[1].rotation_amplitude = 1.0f;
        blend_b = 1.0f;
    }
    float blend_c = layers[2].rotation_amplitude;
    if ( layers[2].rotation_amplitude == 0.0f && layers[3].rotation_amplitude == 0.0f )
        layers[3].rotation_amplitude = 1.0f;

    float game_time = (float)global_frame_parameters.game_time_sec;
    for ( int16_t layer_index = 0; layer_index < 4; ++layer_index )
    {
        water_bumpmap_layer *layer = &layers[layer_index];
        double cos_angle = cos(layer->rotation_angle);
        double sin_angle = sin(layer->rotation_angle);

        float *constants = &texture_transforms[8 * layer_index];
        constants[0] = (float)layer->uv_scale;
        constants[1] = 0.0f;
        constants[2] = 0.0f;
        constants[3] = (layer->rotation_amplitude * (float)cos_angle) * game_time + layer->u_base;
        constants[4] = 0.0f;
        constants[5] = (float)layer->uv_scale;
        constants[6] = 0.0f;
        constants[7] = (layer->rotation_amplitude * (float)sin_angle) * game_time + layer->v_base;
    }
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, texture_transforms, 8, (uint64_t)7 << 58);

    pixel_constants[3] = blend_a / (blend_b + blend_a);
    pixel_constants[7] = blend_c / (layers[3].rotation_amplitude + blend_c);
    pixel_constants[11] = (blend_b + blend_a) / ((layers[3].rotation_amplitude + blend_c) + blend_b + blend_a);
    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_none);

    if ( layer_count > 0 )
    {
        for ( int16_t stage_index = 0; stage_index < layer_count; ++stage_index )
        {
            /* bump base color; RGB is provably fixed here (see file comment, deviation 5) */
            int16_t tint_source_count = water->water.ripple_mipmap_levels;
            pixel_constants[12] = 0.50196081f;
            pixel_constants[13] = 0.50196081f;
            pixel_constants[14] = 1.0f;
            pixel_constants[15] = tint_source_count <= 1
                    ? 0.0f
                    : water->water.ripple_mipmap_fade_factor * ((float)stage_index / (float)(tint_source_count - 1));

            rasterizer_set_target(8, stage_index, 0, 0, 0);

            for ( int16_t texture_stage = 0; texture_stage < 4; ++texture_stage )
            {
                int bitmap_group_index = texture_stage >= reflection_count
                        ? -1
                        : water->water.ripple_maps.index;
                rasterizer_set_texture_for_effect(texture_stage, 0, 3, bitmap_group_index,
                        layers[texture_stage].bitmap_index, effect_shader);
            }

            unsigned int pass_count;
            ID3DXEffect_Begin(effect_shader->effect, &pass_count, 3);
            for ( unsigned int pass = 0; pass < pass_count; ++pass )
            {
                ID3DXEffect_BeginPass(effect_shader->effect, pass);
                D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 4,
                        (uint64_t)1 << 63);
                D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_0,
                        sizeof(dynamic_screen_vertex));
                ID3DXEffect_EndPass(effect_shader->effect);
            }
            ID3DXEffect_End(effect_shader->effect);
        }
    }

    rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
}
