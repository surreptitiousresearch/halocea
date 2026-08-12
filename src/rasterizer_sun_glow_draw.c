/* rasterizer_sun_glow_draw @0x83801BC0 — draw the sun/lens-flare "glow" bloom overlay for one flare.
 *
 * Projects the flare's occlusion point to screen space, and if it lands on-screen draws a fixed 64x64-pixel
 * (center +/-32) sprite through the sun-glow D3DX effect (shader 77): pass 1 masks the glow shape, pass 2
 * composites the glow bitmap (global_rasterizer_data->glow) textured over it at the projected depth. When the
 * effect owns a render target it then copies the masked region into scratch targets 6/7, convolves them
 * (4 passes) into a blurred glow, and finally additively blends 16 concentric, outward-expanding quads of
 * that blurred texture with alpha falling off as glow_intensity/(i+1) — the radial "sun rays" bloom. The whole
 * sprite's brightness is gated by glow_intensity, a 0..1 ramp of how closely the flare direction aligns with
 * the view forward axis (cos(angle) remapped from cos(45deg)..1 -> 0..1, clamped).
 *
 * DEVIATIONS (decompiler emitted "local variable allocation has failed"; the __int128/OVERLAPPED/long-double
 * puns below were all register-allocator noise, resolved against the disassembly 0x83801BC0-0x83802670):
 *  - Single parameter `flare` (prologue saves only r3->r29); the decompiler's a2/a3/a4 are phantom.
 *  - rasterizer_project_billboard: `radius` is a float arg (arg2 -> f1), so per the PPC ABI its GPR slot r4
 *    is RESERVED/skipped. The decompiler bound the skipped-r4 leftover (which merely held flare->definition,
 *    loaded to fetch occlusion_radius) as a bogus 3rd "projected_center" pointer. The real args are
 *    (&occlusion_point, definition->occlusion_radius, &projected_center, &projected_axes); projected_center
 *    receives {screen_x, screen_y, depth} and projected_axes is written-but-unused here. `depth` (the
 *    decompiler's stray v74) is projected_center.n[2], used as the composited glow quad's z.
 *  - The per-sampler `m_Constants.Fetch[N].Texture.dword[...]` / `m_Pending.m_Mask[3]` raw pokes are the
 *    compiler-inlined sampler-state setters, restored to their named helper calls (same convention as the
 *    sibling rasterizer_plasma_energy_draw.c). Each `__ROL4__(1,11)&0x1C00` is `2<<10` strength-reduced:
 *    verified against D3DDevice_SetSamplerState_AddressU_Inline's body (`(Value<<10)&0x1C00`), so Value==2
 *    (CLAMP) for both AddressU (mask 0x1C00) and AddressV (mask 0xE000); dword[3] bit 23 poke is
 *    SeparateZFilterEnable with Value==1.
 *  - The two SetVertexShaderConstantFN uploads build a pixels->NDC ortho projection from the int16
 *    viewport_bounds (width = x1-x0, height = y1-y0) via fcfid int->float; the garbled Vector4fCount /
 *    PendingMask fragments are the plain small integers 5 and (unsigned __int64)3<<59.
 *  - glow_intensity's four cos(0.7853981633974483 == pi/4) calls (recomputed per branch in the binary) are
 *    one constant, hoisted to cos_quarter_pi here.
 *  - The __int128 loop-counter (LODWORD/HIDWORD split of `i` vs `i+1`) is one int: `i` drives the quad
 *    expansion (i*5 - 4 px) and `i+1` the alpha divisor. */

#include <stdint.h>
#include <math.h>

#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/lens_flare_definition.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/dynamic_unlit_vertex.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_rectangle2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/_D3DTEXTUREFILTERTYPE.h"
#include "headers/_D3DBLENDOP.h"
#include "headers/_D3DCULL.h"
#include "headers/_D3DBLEND.h"
#include "headers/_D3DTEXTUREADDRESS.h"
#include "headers/_D3DCMPFUNC.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_vertex_shader_index.h"


/* cos provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* floor provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern float normalize3d(real_vector3d *v);
extern real_vector3d * uncompress_int32_to_real_vector3d(real_vector3d *result, unsigned int i);
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern uint8_t rasterizer_project_billboard(const real_point3d *point, float radius, real_point3d *projected_center, real_vector2d *projected_axes);
extern void rasterizer_sun_glow_copy_source(int16_t target, const real_rectangle2d *bounds);
extern int16_t rasterizer_sun_glow_convolve(int16_t primary_target, int16_t secondary_target, int16_t passes);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);

extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int sampler, unsigned int value);
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
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int vertex_stride);

/* Clamp texture addressing + point filtering + per-slot Z filtering for one sampler (compiler-inlined
   sampler-state setter sequence). */
static void rasterizer_sun_glow_setup_sampler(unsigned int sampler)
{
    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, sampler, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, sampler, D3DTADDRESS_CLAMP);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, sampler, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, sampler, D3DTEXF_LINEAR);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, sampler, 1);
}

void rasterizer_sun_glow_draw(const rasterizer_lens_flare_submit_parameters *flare)
{
    const render_camera *camera = &global_window_parameters.camera;

    rasterizer_sun_glow_setup_sampler(0);

    /* Direction from camera to the flare's light source, normalized. */
    real_vector3d light_direction;
    light_direction.n[0] = flare->position.n[0] - camera->position.n[0];
    light_direction.n[1] = flare->position.n[1] - camera->position.n[1];
    light_direction.n[2] = flare->position.n[2] - camera->position.n[2];
    normalize3d(&light_direction);

    /* Intensity ramp: cos(angle between light dir and view forward) remapped from cos(45deg)..1 -> 0..1. */
    float alignment = camera->forward.n[0] * light_direction.n[0]
            + camera->forward.n[2] * light_direction.n[2]
            + camera->forward.n[1] * light_direction.n[1];
    float cos_quarter_pi = (float)cos(0.7853981633974483);
    float glow_intensity;
    float intensity_ramp = (alignment - cos_quarter_pi) / (1.0f - cos_quarter_pi);
    if ( intensity_ramp < 0.0f )
        glow_intensity = 0.0f;
    else if ( intensity_ramp > 1.0f )
        glow_intensity = 1.0f;
    else
        glow_intensity = intensity_ramp;

    /* Pixels->NDC orthographic projection matrix built from the int16 viewport bounds. */
    float viewport_width = (float)(short)(camera->viewport_bounds.__s1.x1 - camera->viewport_bounds.__s1.x0);
    float viewport_height = (float)(short)(camera->viewport_bounds.__s1.y1 - camera->viewport_bounds.__s1.y0);
    float projection[20];
    projection[0]  = (1.0f / viewport_width) * 2.0f;
    projection[1]  = 0.0f;
    projection[2]  = 0.0f;
    projection[3]  = -1.0f - (1.0f / viewport_width);
    projection[4]  = 0.0f;
    projection[5]  = (1.0f / viewport_height) * -2.0f;
    projection[6]  = 0.0f;
    projection[7]  = (1.0f / viewport_height) + 1.0f;
    projection[8]  = 0.0f;
    projection[9]  = 0.0f;
    projection[10] = 1.0f;
    projection[11] = 0.0f;
    projection[12] = 0.0f;
    projection[13] = 0.0f;
    projection[14] = 0.0f;
    projection[15] = 1.0f;
    projection[16] = 0.0f;
    projection[17] = 0.0f;
    projection[18] = 0.0f;
    projection[19] = 1.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, projection, 5, (uint64_t)3 << 59);

    /* Occlusion sample point = light position offset along its own (uncompressed) direction. */
    real_vector3d flare_direction;
    uncompress_int32_to_real_vector3d(&flare_direction, flare->compressed_direction);
    float occlusion_radius = flare->definition->occlusion_radius;
    real_point3d occlusion_point;
    occlusion_point.n[0] = occlusion_radius * flare_direction.n[0] + flare->position.n[0];
    occlusion_point.n[1] = occlusion_radius * flare_direction.n[1] + flare->position.n[1];
    occlusion_point.n[2] = occlusion_radius * flare_direction.n[2] + flare->position.n[2];

    real_point3d projected_center;   /* {screen_x, screen_y, depth} */
    real_vector2d projected_axes;    /* written by callee, unused here */
    if ( !rasterizer_project_billboard(&occlusion_point, flare->definition->occlusion_radius,
                                       &projected_center, &projected_axes) )
        return;

    /* Snap the projected center to a pixel and build the fixed 64x64 (center +/-32) sprite bounds. */
    float center_x = (float)floor(projected_center.n[0] + 0.5f);
    float center_y = (float)floor(projected_center.n[1] + 0.5f);
    float depth = projected_center.n[2];
    float left = center_x - 32.0f;
    float right = center_x + 32.0f;
    float top = center_y - 32.0f;
    float bottom = center_y + 32.0f;

    rasterizer_dx9_shader *dxeffect_shader = rasterizer_shader_select(_dxshader_sun_glow_draw);

    rasterizer_sun_glow_setup_sampler(1);
    rasterizer_sun_glow_setup_sampler(2);
    rasterizer_sun_glow_setup_sampler(3);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_unlit));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_lens_flare));
    D3DDevice_SetPixelShader(global_d3d_device, 0);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 8);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    /* Pass 1: mask the glow shape (untextured, z = 0). */
    vertex_data_3[0].position.n[0] = left;
    vertex_data_3[0].position.n[1] = top;
    vertex_data_3[0].position.n[2] = 0.0f;
    vertex_data_3[1].position.n[0] = right;
    vertex_data_3[1].position.n[1] = top;
    vertex_data_3[1].position.n[2] = 0.0f;
    vertex_data_3[2].position.n[0] = right;
    vertex_data_3[2].position.n[1] = bottom;
    vertex_data_3[2].position.n[2] = 0.0f;
    vertex_data_3[3].position.n[0] = left;
    vertex_data_3[3].position.n[1] = bottom;
    vertex_data_3[3].position.n[2] = 0.0f;

    unsigned int pass_count[2];
    ID3DXEffect_Begin(dxeffect_shader->effect, pass_count, 3);
    ID3DXEffect_BeginPass(dxeffect_shader->effect, 1);
    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_3, 0x18);
    ID3DXEffect_EndPass(dxeffect_shader->effect);
    ID3DXEffect_End(dxeffect_shader->effect);

    /* Pass 2: composite the glow bitmap over the mask at the projected depth (textured, white). */
    rasterizer_set_texture_direct_for_effect(0, global_rasterizer_data->glow.index, 0, dxeffect_shader);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 8);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_ZFunc(global_d3d_device, D3DCMP_LESSEQUAL);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

    vertex_data_3[0].position.n[0] = left;
    vertex_data_3[0].position.n[1] = top;
    vertex_data_3[0].position.n[2] = depth;
    vertex_data_3[0].texcoord.n[0] = 0.0f;
    vertex_data_3[0].texcoord.n[1] = 0.0f;
    vertex_data_3[0].color = -1;
    vertex_data_3[1].position.n[0] = right;
    vertex_data_3[1].position.n[1] = top;
    vertex_data_3[1].position.n[2] = depth;
    vertex_data_3[1].texcoord.n[0] = 1.0f;
    vertex_data_3[1].texcoord.n[1] = 0.0f;
    vertex_data_3[1].color = -1;
    vertex_data_3[2].position.n[0] = right;
    vertex_data_3[2].position.n[1] = bottom;
    vertex_data_3[2].position.n[2] = depth;
    vertex_data_3[2].texcoord.n[0] = 1.0f;
    vertex_data_3[2].texcoord.n[1] = 1.0f;
    vertex_data_3[2].color = -1;
    vertex_data_3[3].position.n[0] = left;
    vertex_data_3[3].position.n[1] = bottom;
    vertex_data_3[3].position.n[2] = depth;
    vertex_data_3[3].texcoord.n[0] = 0.0f;
    vertex_data_3[3].texcoord.n[1] = 1.0f;
    vertex_data_3[3].color = -1;

    ID3DXEffect_Begin(dxeffect_shader->effect, pass_count, 3);
    ID3DXEffect_BeginPass(dxeffect_shader->effect, 2);
    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_3, 0x18);
    ID3DXEffect_EndPass(dxeffect_shader->effect);
    ID3DXEffect_End(dxeffect_shader->effect);

    if ( !dxeffect_shader->effect )
        return;

    /* Convolve the masked region into a blurred glow, then additively bloom expanding rings of it. */
    real_rectangle2d glow_bounds;
    glow_bounds.n[0] = left;
    glow_bounds.n[1] = right;
    glow_bounds.n[2] = top;
    glow_bounds.n[3] = bottom;
    rasterizer_sun_glow_copy_source(_rasterizer_target_sun_glow_primary, &glow_bounds);
    rasterizer_sun_glow_copy_source(_rasterizer_target_sun_glow_secondary, &glow_bounds);
    int16_t blurred_target = rasterizer_sun_glow_convolve(_rasterizer_target_sun_glow_primary, _rasterizer_target_sun_glow_secondary, 4);

    float convolve_projection[20];
    convolve_projection[0]  = (1.0f / viewport_width) * 2.0f;
    convolve_projection[1]  = 0.0f;
    convolve_projection[2]  = 0.0f;
    convolve_projection[3]  = -1.0f - (1.0f / viewport_width);
    convolve_projection[4]  = 0.0f;
    convolve_projection[5]  = (1.0f / viewport_height) * -2.0f;
    convolve_projection[6]  = 0.0f;
    convolve_projection[7]  = (1.0f / viewport_height) + 1.0f;
    convolve_projection[8]  = 0.0f;
    convolve_projection[9]  = 0.0f;
    convolve_projection[10] = 0.0f;
    convolve_projection[11] = 0.5f;
    convolve_projection[12] = 0.0f;
    convolve_projection[13] = 0.0f;
    convolve_projection[14] = 0.0f;
    convolve_projection[15] = 1.0f;
    convolve_projection[16] = 1.0f;
    convolve_projection[17] = 1.0f;
    convolve_projection[18] = 0.0f;
    convolve_projection[19] = 1.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, convolve_projection, 5, (uint64_t)3 << 59);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_unlit));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_lens_flare));
    rasterizer_set_target_as_texture_for_effect(0, blurred_target, 0, dxeffect_shader);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 1);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_CCW);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, D3DBLEND_SRCALPHA);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, D3DBLEND_ONE);
    D3DDevice_SetRenderState_BlendOp(global_d3d_device, D3DBLENDOP_ADD);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    for ( int ring = 0; ring < 16; ++ring )
    {
        float expansion = (float)ring * 0.0625f * 80.0f - 4.0f;   /* ring*5 - 4 pixels */
        int alpha = (int)(glow_intensity / (float)(ring + 1) * 255.0f);
        unsigned int color = ((unsigned int)alpha << 24) | 0xFFFFFF;
        float ring_left = left - expansion;
        float ring_right = right + expansion;
        float ring_top = top - expansion;
        float ring_bottom = bottom + expansion;

        vertex_data_3[0].position.n[0] = ring_left;
        vertex_data_3[0].position.n[1] = ring_top;
        vertex_data_3[0].position.n[2] = 0.0f;
        vertex_data_3[0].texcoord.n[0] = 0.0f;
        vertex_data_3[0].texcoord.n[1] = 0.0f;
        vertex_data_3[0].color = color;
        vertex_data_3[1].position.n[0] = ring_right;
        vertex_data_3[1].position.n[1] = ring_top;
        vertex_data_3[1].position.n[2] = 0.0f;
        vertex_data_3[1].texcoord.n[0] = 1.0f;
        vertex_data_3[1].texcoord.n[1] = 0.0f;
        vertex_data_3[1].color = color;
        vertex_data_3[2].position.n[0] = ring_right;
        vertex_data_3[2].position.n[1] = ring_bottom;
        vertex_data_3[2].position.n[2] = 0.0f;
        vertex_data_3[2].texcoord.n[0] = 1.0f;
        vertex_data_3[2].texcoord.n[1] = 1.0f;
        vertex_data_3[2].color = color;
        vertex_data_3[3].position.n[0] = ring_left;
        vertex_data_3[3].position.n[1] = ring_bottom;
        vertex_data_3[3].position.n[2] = 0.0f;
        vertex_data_3[3].texcoord.n[0] = 0.0f;
        vertex_data_3[3].texcoord.n[1] = 1.0f;
        vertex_data_3[3].color = color;

        ID3DXEffect_Begin(dxeffect_shader->effect, pass_count, 3);
        ID3DXEffect_BeginPass(dxeffect_shader->effect, 0);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_3, 0x18);
        ID3DXEffect_EndPass(dxeffect_shader->effect);
        ID3DXEffect_End(dxeffect_shader->effect);
    }
}
