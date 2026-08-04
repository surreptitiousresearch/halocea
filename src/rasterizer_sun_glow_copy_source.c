/* rasterizer_sun_glow_copy_source @0x83801478 — seed the sun-glow blur chain: copy the glow bitmap into
 * the sun-glow render target through effect 76, applying a texture-coordinate transform that maps the
 * source region `bounds` (in viewport pixels) onto the full-screen quad. Binds render target 1 as sampler
 * stage 0 and the glow bitmap group (global_rasterizer_data->glow) directly, uploads an 8-vector4 vertex
 * constant matrix (two scale/offset rows derived from bounds vs. the camera viewport, then three identity
 * pairs), and draws one full-viewport quad into `target`. When cf_LinearTextureAddressingSun is set the
 * viewport division is cancelled out (texel-space instead of normalized addressing).
 *
 * Sibling of rasterizer_sun_glow_convolve.c / rasterizer_shadow_convolve.c.
 *
 * DEVIATIONS (all disasm-resolved, catalog class 5/9): the stage-0 sampler poke
 * (m_Constants.Fetch[0].Texture dword[3] &= 0xFE7FFFFF, m_Pending.m_Mask[3] |= 0x80000000) is the
 * established SeparateZFilterEnable=0 call; the set_texture_direct stage (rendered SWORD1(v7)) is 1;
 * SetVertexShaderConstantFN's count/mask (rendered 0x1C000000 / an unassigned local) are 8 and 7<<58; the
 * __int128 viewport packing decodes to plain (float)(viewport_width/height) integer-to-float conversions
 * used as the ratio denominators. rasterizer_render_target_get_dimension's outputs are unused here
 * (vestigial) but the call is preserved. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_render_target_get_dimension(int16_t target, unsigned int *width, unsigned int *height);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int stride);

void rasterizer_sun_glow_copy_source(int16_t target, const real_rectangle2d *bounds)
{
    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_sun_glow_convolve);

    rasterizer_set_target_as_texture_for_effect(0, 1, 0, shader);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);
    rasterizer_set_texture_direct_for_effect(1, global_rasterizer_data->glow.index, 0, shader);

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xF);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(0));

    float extent_u = bounds->n[1] - bounds->n[0];
    float base_u = bounds->n[0];
    float extent_v = bounds->n[3] - bounds->n[2];
    float base_v = bounds->n[2];

    float viewport_width = (float)(__int16)(global_window_parameters.camera.viewport_bounds.n[2]
                                            - global_window_parameters.camera.viewport_bounds.n[0]);
    float viewport_height = (float)(__int16)(global_window_parameters.camera.viewport_bounds.n[3]
                                             - global_window_parameters.camera.viewport_bounds.n[1]);

    float scale_u = extent_u / viewport_height;
    float offset_u = base_u / viewport_height;
    float scale_v = extent_v / viewport_width;
    float offset_v = base_v / viewport_width;

    unsigned int target_width, target_height; /* vestigial — result unused */
    rasterizer_render_target_get_dimension(1, &target_width, &target_height);

    if ( cf_LinearTextureAddressingSun )
    {
        scale_u = viewport_height * scale_u;
        offset_u = viewport_height * offset_u;
        scale_v = viewport_width * scale_v;
        offset_v = viewport_width * offset_v;
    }

    float texture_transform[32] = {
        scale_u, 0.0f, 0.0f, offset_u,
        0.0f, scale_v, 0.0f, offset_v,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
    };
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, texture_transform, 8,
            (unsigned __int64)7 << 58);

    rasterizer_set_target(target, 0, 0, 0, 0);

    if ( shader && shader->effect )
    {
        vertex_data_3[0].position = (real_point3d){ { -1.015625f, 1.015625f, 0.0f } };
        vertex_data_3[0].texcoord = (real_point2d){ { 0.0f, 0.0f } };
        vertex_data_3[0].color = 0xFFFFFFFF;
        vertex_data_3[1].position = (real_point3d){ { 0.984375f, 1.015625f, 0.0f } };
        vertex_data_3[1].texcoord = (real_point2d){ { 1.0f, 0.0f } };
        vertex_data_3[1].color = 0xFFFFFFFF;
        vertex_data_3[2].position = (real_point3d){ { 0.984375f, -0.984375f, 0.0f } };
        vertex_data_3[2].texcoord = (real_point2d){ { 1.0f, 1.0f } };
        vertex_data_3[2].color = 0xFFFFFFFF;
        vertex_data_3[3].position = (real_point3d){ { -1.015625f, -0.984375f, 0.0f } };
        vertex_data_3[3].texcoord = (real_point2d){ { 0.0f, 1.0f } };
        vertex_data_3[3].color = 0xFFFFFFFF;

        unsigned int pass_count[4];
        ID3DXEffect_Begin(shader->effect, pass_count, 3);
        ID3DXEffect_BeginPass(shader->effect, 0);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_3,
                sizeof(dynamic_screen_vertex));
        ID3DXEffect_EndPass(shader->effect);
        ID3DXEffect_End(shader->effect);
    }

    rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
}
