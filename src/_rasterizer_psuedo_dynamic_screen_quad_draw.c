/* _rasterizer_psuedo_dynamic_screen_quad_draw @0x837A8FD8 (2552 bytes) — the real implementation behind the
 * rasterizer_psuedo_dynamic_screen_quad_draw linker thunk. Debug-only path (gated on
 * rasterizer_debug_options.draw_dynamic_screen_geometry && rasterizer_target == _rasterizer_target_render_primary): builds a screen-space
 * pixel->NDC transform plus per-map UV scale/offset/anchor vertex-shader constants (c13..c17, c18..c23),
 * binds up to three texture stages with wrap/point-sample sampler state, then draws the quad either through
 * the "meter" two-color-bar effect (when parameters->meter_parameters is set) or through the normal
 * tint/fade/plasma-fade blended path (effect 122 for a single map, or a map0/1/2 blend-function-selected
 * effect for two-or-three maps), delegating the actual Begin/BeginPass/DrawVerticesUP/EndPass/End sequence
 * to _rasterizer_psuedo_dynamic_screen_quad_draw_fx.
 *
 * REG-ALLOC-FAIL: the decompiler reports "local variable allocation has failed" on this function — PPC's
 * store-as-int64/load-as-double int-to-float idiom (used for the width/height conversions) gets fused with
 * unrelated OVERLAPPED locals. All arithmetic below was re-derived from disasm_range(0x837A8FD8, 0x837A9420)
 * rather than trusted from the garbled decompiler output. Findings:
 *   - camera.viewport_bounds is {y0,x0,y1,x1} (see rectangle2d.h): the decompiler's raw n[3]-n[1] is
 *     x1-x0 (viewport WIDTH), and n[2]-n[0] is y1-y0 (viewport HEIGHT) — the reverse of what the garbled
 *     OVERLAPPED variable names in the decompiler dump suggested. Confirmed by which one x-offset vs.
 *     y-offset divides by (837a90d4..837a9150).
 *   - The screen transform is the standard D3D9 "pixel snap" NDC mapping: x_ndc = px*(2/w) - (1+1/w),
 *     y_ndc = py*(-2/h) + (1+1/h), each with an optional parameters->offset term folded in.
 *   - The two m_Constants.Fetch[stage]/m_Pending pokes for the wrap state (0x837A9428-0x837A94A4) match the
 *     established D3DDevice_SetSamplerState_AddressU_Inline/_AddressV_Inline idiom used elsewhere in this
 *     corpus (e.g. rasterizer_transparent_geometry_group_draw.c): same 0x800 (U, shift 10, mask 0x1C00) and
 *     0x4000 (V, shift 13, mask 0xE000) literal bit patterns as the `(power << 10) & 0x800` /
 *     `(power << 12) & 0x4000` precedent, here gated by parameters->map_wrapped[stage] instead of `power`:
 *     wrapped != 0 -> 0 (hardware default = wrap), wrapped == 0 -> the literal (clamp) bits.
 *   - D3DDevice_SetSamplerState_MagFilter/MinFilter are real explicit calls in disasm (not inlined), so the
 *     decompiler's `parameters->point_sampled == 0` expression for their Value argument is trusted as-is.
 *   - D3DDevice_SetVertexShaderConstantFN counts/masks (decompiler drops these to uninitialized OVERLAPPED
 *     reads) are, per disasm: c13, count 5, mask 3<<59; c18(0x12), count 6, mask 3<<58.
 *   - real_argb_color is {alpha, r, g, b} (n[0..3]); the plasma_fade packing below (n[1],n[2],n[3],n[0]) is
 *     therefore (r,g,b,alpha) — consistent with the (tint.r,tint.g,tint.b,fade) grouping used for the other
 *     three vec4s in the same constant array.
 *   - parameters->meter_parameters is read only as a raw byte array (index 0,1..3,8..11) in the binary, so
 *     it is treated here as `const unsigned __int8 *` per the decompiler's own typing rather than invented
 *     as a struct; headers/rasterizer_dynamic_screen_geometry_parameters.h leaves the pointee unresolved. */

#include <stdint.h>
#include <string.h>
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"
#include "headers/rasterizer_target.h"
#include "headers/shader_framebuffer_blend_function.h"
#include "headers/blam_data_globals.h"



extern void rasterizer_set_framebuffer_blend_function(int16_t framebuffer_blend_function);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern uint8_t _rasterizer_psuedo_dynamic_screen_quad_draw_fx(const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *verts, rasterizer_dx9_shader *dxeffect_shader, const float *psh_constants);

extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaRef(D3DDevice *device, unsigned int ref);
extern void D3DDevice_SetRenderState_AlphaFunc(D3DDevice *device, unsigned int func);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
    const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);

void _rasterizer_psuedo_dynamic_screen_quad_draw(
    const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices)
{
    if ( !rasterizer_debug_options.draw_dynamic_screen_geometry || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary )
        return;

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 0);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7u);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 1u);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
    rasterizer_set_framebuffer_blend_function(parameters->framebuffer_blend_function);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen2));

    /* NOTE: rectangle2d is {y0,x0,y1,x1} — width is x1-x0 (n[3]-n[1]), height is y1-y0 (n[2]-n[0]). */
    float viewport_width = (float)(global_window_parameters.camera.viewport_bounds.__s1.x1
                                  - global_window_parameters.camera.viewport_bounds.__s1.x0);
    float viewport_height = (float)(global_window_parameters.camera.viewport_bounds.__s1.y1
                                   - global_window_parameters.camera.viewport_bounds.__s1.y0);

    real_vector2d *offset = parameters->offset;
    float offset_x_ndc = offset ? (offset->n[0] * 2.0f) / viewport_width : 0.0f;
    float offset_y_ndc = offset ? (offset->n[1] * -2.0f) / viewport_height : 0.0f;

    /* c13..c17 (5 vec4): screen pixel->NDC transform (rows 0-3) + map0 UV scale (row 4). */
    float vs_screen_transform[20];
    vs_screen_transform[0] = (1.0f / viewport_width) * 2.0f;
    vs_screen_transform[1] = 0.0f;
    vs_screen_transform[2] = 0.0f;
    vs_screen_transform[3] = offset_x_ndc - ((1.0f / viewport_width) + 1.0f);
    vs_screen_transform[4] = 0.0f;
    vs_screen_transform[5] = (1.0f / viewport_height) * -2.0f;
    vs_screen_transform[6] = 0.0f;
    vs_screen_transform[7] = ((1.0f / viewport_height) + offset_y_ndc) + 1.0f;
    vs_screen_transform[8] = 0.0f;
    vs_screen_transform[9] = 0.0f;
    vs_screen_transform[10] = 0.0f;
    vs_screen_transform[11] = 0.5f;
    vs_screen_transform[12] = 0.0f;
    vs_screen_transform[13] = 0.0f;
    vs_screen_transform[14] = 0.0f;
    vs_screen_transform[15] = 1.0f;
    vs_screen_transform[16] = parameters->map_texture_scale[0].n[0];
    vs_screen_transform[17] = parameters->map_texture_scale[0].n[1];
    vs_screen_transform[18] = 0.0f;
    vs_screen_transform[19] = 1.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vs_screen_transform, 5u, (uint64_t)3 << 59);

    /* c18..c23 (6 vec4): map1/map2 UV scale, per-map screen anchor flags, per-map UV offset, per-map scale. */
    float vs_map_params[24];
    vs_map_params[0] = parameters->map_texture_scale[1].n[0];
    vs_map_params[1] = parameters->map_texture_scale[1].n[1];
    vs_map_params[2] = parameters->map_texture_scale[2].n[0];
    vs_map_params[3] = parameters->map_texture_scale[2].n[1];
    vs_map_params[4] = parameters->map_anchor_screen[0] ? 1.0f : 0.0f;
    vs_map_params[5] = parameters->map_anchor_screen[0] ? 0.0f : 1.0f;
    vs_map_params[6] = parameters->map_anchor_screen[1] ? 1.0f : 0.0f;
    vs_map_params[7] = parameters->map_anchor_screen[1] ? 0.0f : 1.0f;
    vs_map_params[8] = parameters->map_anchor_screen[2] ? 1.0f : 0.0f;
    vs_map_params[9] = parameters->map_anchor_screen[2] ? 0.0f : 1.0f;
    vs_map_params[10] = parameters->map_offset[0] ? parameters->map_offset[0]->n[0] : 0.0f;
    vs_map_params[11] = parameters->map_offset[0] ? parameters->map_offset[0]->n[1] : 0.0f;
    vs_map_params[12] = parameters->map_offset[1] ? parameters->map_offset[1]->n[0] : 0.0f;
    vs_map_params[13] = parameters->map_offset[1] ? parameters->map_offset[1]->n[1] : 0.0f;
    vs_map_params[14] = parameters->map_offset[2] ? parameters->map_offset[2]->n[0] : 0.0f;
    vs_map_params[15] = parameters->map_offset[2] ? parameters->map_offset[2]->n[1] : 0.0f;
    vs_map_params[16] = parameters->map_scale[0].n[0];
    vs_map_params[17] = parameters->map_scale[0].n[1];
    vs_map_params[18] = parameters->map_scale[1].n[0];
    vs_map_params[19] = parameters->map_scale[1].n[1];
    vs_map_params[20] = parameters->map_scale[2].n[0];
    vs_map_params[21] = parameters->map_scale[2].n[1];
    vs_map_params[22] = 0.0f;
    vs_map_params[23] = 0.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x12, vs_map_params, 6u, (uint64_t)3 << 58);

    /* Bind up to three texture stages, stopping at the first unset map. Wrap state pokes match the
     * established AddressU/V_Inline idiom: wrapped != 0 -> 0 (hardware default/wrap), wrapped == 0 -> the
     * clamp bit pattern (0x800 for U, 0x4000 for V; see file header). */
    for ( int stage = 0; stage < 3; ++stage )
    {
        if ( !parameters->map[stage] )
            break;

        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, stage,
            parameters->map_wrapped[stage] != 0 ? 0 : 0x800);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, stage,
            parameters->map_wrapped[stage] != 0 ? 0 : 0x4000);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, stage, parameters->point_sampled == 0);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, stage, parameters->point_sampled == 0);
    }

    float pixel_shader_constants[24];
    pixel_shader_constants[0] = 0.0f;
    memset(&pixel_shader_constants[1], 0, 0x5Cu);

    if ( parameters->meter_parameters )
    {
        /* "Meter" path: a two-color-bar effect. meter_parameters is only ever indexed as raw bytes in the
         * binary (alpha ref + two RGBA colors quantized to [0,255]); its pointee layout is not resolved. */
        const uint8_t *meter = (const uint8_t *)parameters->meter_parameters;
        uint8_t meter_alpha_reference = meter[0];   /* byte 0: alpha ref, doubles as pass-1 alpha */

        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_AlphaFunc(global_d3d_device, 3u);
        D3DDevice_SetRenderState_AlphaRef(global_d3d_device, meter_alpha_reference);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xFu);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 1u);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, 1u);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);

        /* Pass 1: bytes 1..3 = empty-bar RGB, quantized [0,255]; * (1/255) to normalize. */
        pixel_shader_constants[0] = meter[1] * 0.00392156862745098f;   /* 1/255 */
        pixel_shader_constants[1] = meter[2] * 0.00392156862745098f;
        pixel_shader_constants[2] = meter[3] * 0.00392156862745098f;
        pixel_shader_constants[3] = meter_alpha_reference * 0.00392156862745098f;
        pixel_shader_constants[4] = 1.0f;   /* pass id */
        pixel_shader_constants[5] = 0.0f;

        rasterizer_dx9_shader *meter_shader = rasterizer_shader_select(_dxshader_dyn_geom);
        uint8_t first_pass_ok =
            _rasterizer_psuedo_dynamic_screen_quad_draw_fx(parameters, vertices, meter_shader, pixel_shader_constants) != 0;

        D3DDevice_SetRenderState_AlphaFunc(global_d3d_device, 4u);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xFu);

        /* Pass 2: bytes 9..11 = filled-bar RGB, byte 8 its alpha; same [0,255] quantization. */
        pixel_shader_constants[0] = meter[9] * 0.00392156862745098f;   /* 1/255 */
        pixel_shader_constants[1] = meter[10] * 0.00392156862745098f;
        pixel_shader_constants[2] = meter[11] * 0.00392156862745098f;
        pixel_shader_constants[3] = meter[8] * 0.00392156862745098f;
        pixel_shader_constants[4] = 2.0f;   /* pass id */
        pixel_shader_constants[5] = 0.0f;

        if ( !first_pass_ok )
            return;

        _rasterizer_psuedo_dynamic_screen_quad_draw_fx(parameters, vertices, meter_shader, pixel_shader_constants);
        return;
    }

    if ( !parameters->map[0] )
        return;

    /* Tint/fade/plasma-fade path: pack (tint.r,tint.g,tint.b,fade) per map into c0..c2, plasma_fade as
     * (r,g,b,alpha) into c3 (real_argb_color is {alpha,r,g,b}, so n[1],n[2],n[3],n[0]). */
    const real_rgb_color *tint0 = parameters->map_tint[0] ? parameters->map_tint[0] : global_real_rgb_white;
    float fade0 = parameters->map_fade[0] ? *parameters->map_fade[0] : 1.0f;
    int16_t shader_index;
    rasterizer_dx9_shader *tint_shader;

    if ( parameters->map[1] || parameters->map[2] )
    {
        const real_rgb_color *tint1 = parameters->map_tint[1] ? parameters->map_tint[1] : global_real_rgb_white;
        const real_rgb_color *tint2 = parameters->map_tint[2] ? parameters->map_tint[2] : global_real_rgb_white;
        float fade1 = parameters->map_fade[1] ? *parameters->map_fade[1] : 1.0f;
        float fade2 = parameters->map_fade[2] ? *parameters->map_fade[2] : 1.0f;

        pixel_shader_constants[0] = tint0->n[0];
        pixel_shader_constants[1] = tint0->n[1];
        pixel_shader_constants[2] = tint0->n[2];
        pixel_shader_constants[3] = fade0;
        pixel_shader_constants[4] = tint1->n[0];
        pixel_shader_constants[5] = tint1->n[1];
        pixel_shader_constants[6] = tint1->n[2];
        pixel_shader_constants[7] = fade1;
        pixel_shader_constants[8] = tint2->n[0];
        pixel_shader_constants[9] = tint2->n[1];
        pixel_shader_constants[10] = tint2->n[2];
        pixel_shader_constants[11] = fade2;
        pixel_shader_constants[12] = parameters->plasma_fade.n[1];
        pixel_shader_constants[13] = parameters->plasma_fade.n[2];
        pixel_shader_constants[14] = parameters->plasma_fade.n[3];
        pixel_shader_constants[15] = parameters->plasma_fade.n[0];

        /* map0->map1 blend function selects a shader-table offset base (0 = none/default); map1->map2
         * blend function adds a further offset on top, only when the corresponding map is present. */
        shader_index = 0;
        if ( parameters->map[1]
          && parameters->map0_to_1_blend_function >= _shader_framebuffer_blend_function_multiply
          && parameters->map0_to_1_blend_function <= _shader_framebuffer_blend_function_min )
        {
            /* shader_index results are protocol-local rasterizer_dx9 shader-table offsets (kept raw). */
            switch ( parameters->map0_to_1_blend_function )
            {
                case _shader_framebuffer_blend_function_multiply:         shader_index = 59; break;
                case _shader_framebuffer_blend_function_double_multiply:  shader_index = 69; break;
                case _shader_framebuffer_blend_function_add:              shader_index = 64; break;
                case _shader_framebuffer_blend_function_reverse_subtract: shader_index = 54; break;
                default: shader_index = 0; break;
            }
        }
        if ( parameters->map[2]
          && parameters->map1_to_2_blend_function >= _shader_framebuffer_blend_function_multiply
          && parameters->map1_to_2_blend_function <= _shader_framebuffer_blend_function_reverse_subtract )
        {
            switch ( parameters->map1_to_2_blend_function )
            {
                case _shader_framebuffer_blend_function_multiply:        shader_index += 2; break;
                case _shader_framebuffer_blend_function_double_multiply: shader_index += 4; break;
                case _shader_framebuffer_blend_function_add:             shader_index += 3; break;
                default: shader_index += 1; break;
            }
        }
        tint_shader = rasterizer_shader_select(shader_index);
    }
    else
    {
        /* Single-map path: straight tint/fade through effect 122, alpha test disabled. */
        pixel_shader_constants[0] = tint0->n[0];
        pixel_shader_constants[1] = tint0->n[1];
        pixel_shader_constants[2] = tint0->n[2];
        pixel_shader_constants[3] = fade0;
        pixel_shader_constants[4] = 1.0f;
        pixel_shader_constants[5] = 1.0f;
        tint_shader = rasterizer_shader_select(_dxshader_dyn_geom);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    }

    if ( tint_shader && tint_shader->effect )
        _rasterizer_psuedo_dynamic_screen_quad_draw_fx(parameters, vertices, tint_shader, pixel_shader_constants);
}
