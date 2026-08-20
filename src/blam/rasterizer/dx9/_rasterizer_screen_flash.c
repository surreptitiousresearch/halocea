/* _rasterizer_screen_flash @0x83687110 — draws the full-screen color-flash/tint overlay
 * (global_window_parameters.screen_flash) as a single alpha-blended quad covering the viewport, when the
 * screen-flash debug toggle is on and a flash is active (type != 0).
 *
 * Selects one of 6 blend modes by `screen_flash.type`: lighten/darken/max/min/invert use a fixed or
 * intensity-scaled D3D blend factor; any other in-range type (only 6 is actually reachable here) uses a
 * "tint" technique with an inverted-color blend factor. Uploads a viewport-to-NDC transform (5 vector4
 * vertex-shader constants) and the flash color in both straight and inverted form (2 vector4 pixel-shader
 * constants), then draws a 4-vertex triangle fan covering the full viewport.
 *
 * DEVIATION: "local variable allocation failed"-class corruption split what is really one contiguous
 * 24-float vertex array (4 vertices x 6 floats, matching the `0x18`-byte stride passed to
 * D3DDevice_DrawVerticesUP) into a dozen unrelated-looking locals (`v23[6]`, `v24`..`v41`) plus 3 stray
 * `int` locals (`v27`/`v33`/`v39`) that land on the vertex color float slot. Rebuilt as a
 * `screen_flash_vertex vertices[4]` array from the exact per-slot byte offsets (all in
 * `[sp+0xD0, sp+0x130)`, confirmed contiguous and matching the draw's `sizeof(screen_flash_vertex)*4`
 * stride/count).
 *
 * DEVIATION: the decompiler shows vertex 0's color slot as the literal float `NAN`, while vertices 1-3
 * show it as the integer `-1` — these are the SAME 0xFFFFFFFF bit pattern (opaque packed white),
 * decompiled inconsistently only because of which store instruction wrote each slot. Modeled uniformly
 * as `0xFFFFFFFF` (an `unsigned int` color field), not "fixed" to a float value.
 *
 * FAITHFUL: the technique-select switch (and the `rasterizer_set_technique` call after it) only runs
 * when `type` is in [1, 6]; any other nonzero type still draws the quad, just with whatever technique was
 * already bound to the effect from a previous call. */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_flash_techniques.h"
#include "headers/render_screen_flash_type.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/blam_data_globals.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern long rasterizer_set_technique(ID3DXEffect *effect, unsigned int technique);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_BlendFactor(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int stride);

typedef struct screen_flash_vertex
{
    float x, y, z;
    unsigned int color;
    float u, v;
} screen_flash_vertex; /* 24 bytes */

static unsigned char color_channel_to_byte(float value)
{
    return (unsigned char)(int)(value * 255.0f);
}

void _rasterizer_screen_flash(void)
{
    if (!rasterizer_debug_options.screen_flash_enabled)
        return;

    render_screen_flash *flash = &global_window_parameters.screen_flash;
    if (!flash->type)
        return;

    float r = flash->color.__s1.rgb.__s1.red * flash->intensity;
    float g = flash->color.__s1.rgb.__s1.green * flash->intensity;
    float b = flash->color.__s1.rgb.__s1.blue * flash->intensity;
    float a = flash->color.__s1.alpha * flash->intensity;
    float inv_r = (1.0f - flash->color.__s1.rgb.__s1.red) * flash->intensity;
    float inv_g = (1.0f - flash->color.__s1.rgb.__s1.green) * flash->intensity;
    float inv_b = (1.0f - flash->color.__s1.rgb.__s1.blue) * flash->intensity;

    /* Straight packed ARGB (used by the max/min/invert blend factors). */
    unsigned int straight_color =
            (((unsigned int)color_channel_to_byte(a) << 8 | color_channel_to_byte(r)) << 8
                    | color_channel_to_byte(g)) << 8
            | color_channel_to_byte(b);
    /* Inverted-color packed ARGB (used by the tint blend factor). */
    unsigned int inverted_color =
            (((unsigned int)color_channel_to_byte(a) << 8 | color_channel_to_byte(inv_r)) << 8
                    | color_channel_to_byte(inv_g)) << 8
            | color_channel_to_byte(inv_b);

    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_screen_flash);
    if (!shader || !shader->effect)
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7u);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);

    if ((unsigned int)(flash->type - 1) <= 5)
    {
        unsigned int technique;
        switch (flash->type)
        {
        case _render_screen_flash_type_lighten:
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_ONE);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVSRCALPHA);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
            technique = hFlashLighten;
            break;
        case _render_screen_flash_type_darken:
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_ONE);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_ONE);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_REVSUBTRACT);
            technique = hFlashDarken;
            break;
        case _render_screen_flash_type_max:
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_INVDESTCOLOR);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVBLENDFACTOR);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_MAX);
            D3DDevice_SetRenderState_BlendFactor(global_d3d_device, straight_color);
            technique = hFlashMax;
            break;
        case _render_screen_flash_type_min:
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_INVDESTCOLOR);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVBLENDFACTOR);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_MIN);
            D3DDevice_SetRenderState_BlendFactor(global_d3d_device, straight_color);
            technique = hFlashMin;
            break;
        case _render_screen_flash_type_invert:
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_INVDESTCOLOR);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVBLENDFACTOR);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
            D3DDevice_SetRenderState_BlendFactor(global_d3d_device, straight_color);
            technique = hFlashInvert;
            break;
        default: /* type == _render_screen_flash_type_tint (the only value left in [1,6] not handled above) */
            D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_ONE);
            D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_INVBLENDFACTOR);
            D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
            D3DDevice_SetRenderState_BlendFactor(global_d3d_device, inverted_color);
            technique = hFlashTint;
            break;
        }
        rasterizer_set_technique(shader->effect, technique);
    }

    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));

    /* Viewport-to-NDC transform (5 vector4 constants at c13). rectangle2d order is (y0, x0, y1, x1). */
    float width = (float)(int16_t)(global_window_parameters.camera.viewport_bounds.__s1.x1
            - global_window_parameters.camera.viewport_bounds.__s1.x0);
    float height = (float)(int16_t)(global_window_parameters.camera.viewport_bounds.__s1.y1
            - global_window_parameters.camera.viewport_bounds.__s1.y0);
    float viewport_transform[20] = {
        2.0f / width,  0.0f,           0.0f, -1.0f - 1.0f / width,
        0.0f,         -2.0f / height,  0.0f,  1.0f / height + 1.0f,
        0.0f,          0.0f,           0.0f,  0.5f,
        0.0f,          0.0f,           0.0f,  1.0f,
        0.0f,          0.0f,           0.0f,  1.0f,
    };
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, viewport_transform, 5u, (uint64_t)3 << 59);

    /* Flash color, straight and inverted (2 vector4 constants at c0).
     * DEVIATION: decompile shows `HIDWORD(v11) = __ROR4__(1, 1)` for the PendingMask0 argument — per the
     * same 32-bit-rotate-into-HIDWORD decompiler artifact documented in _rasterizer_decals_draw.c
     * (__ROR4__(1,N) in HIDWORD == (uint64)1 << (64-N) as a real 64-bit value), that's `(uint64)1 << 63`. */
    float pixel_constants[8] = { r, g, b, a, inv_r, inv_g, inv_b, a };
    D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 2u, (uint64_t)1 << 63);

    /* Full-viewport quad in pixel space, opaque-white vertex color (modulated by the constants above). */
    screen_flash_vertex vertices[4] = {
        { 0.0f,  0.0f,   0.0f, 0xFFFFFFFF, 0.0f, 0.0f },
        { width, 0.0f,   0.0f, 0xFFFFFFFF, 1.0f, 0.0f },
        { width, height, 0.0f, 0xFFFFFFFF, 1.0f, 1.0f },
        { 0.0f,  height, 0.0f, 0xFFFFFFFF, 0.0f, 1.0f },
    };

    unsigned int pass_count[4];
    shader->effect->lpVtbl->Begin(shader->effect, pass_count, 3u);
    for (unsigned int i = 0; i < pass_count[0]; ++i)
    {
        shader->effect->lpVtbl->BeginPass(shader->effect, 0);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4u, vertices, sizeof(screen_flash_vertex));
        shader->effect->lpVtbl->EndPass(shader->effect);
    }
    shader->effect->lpVtbl->End(shader->effect);
}
