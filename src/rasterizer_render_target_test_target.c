/* rasterizer_render_target_test_target @0x836F3900 — debug visualizer: draw one of the global render
 * targets into a screen rectangle so its contents can be inspected. Selects debug effect 122, binds the
 * chosen target's texture, uploads a white pixel-shader constant, sets point sampling / clamp addressing /
 * no cull-blend-alpha-z, builds a single full-rectangle quad (TRIANGLEFAN) from `bounds`, uploads the
 * screen-space orthographic view matrix, and draws. Sibling of
 * rasterizer_active_camouflage_cache_primary_render_target (same select -> set-texture -> sampler-poke ->
 * ortho-const -> full-quad -> effect-pass structure).
 *
 * The decompiler emitted "local variable allocation has failed"; this reconstruction is disasm-driven
 * (0x836F3900-0x836F3D14).
 *
 * DEVIATIONS (all disasm-resolved):
 *  - The m_Constants.Fetch[0] pokes are the established inline sampler helpers (as in the sibling):
 *    AddressU/V = 2 (clamp), Mag/MinFilter = 0 (point), SeparateZFilterEnable = 0.
 *  - The quad's screen X uses the rectangle's x-edges and screen Y its y-edges, but the far corners are
 *    x0+x1 / y0+y1 (a SUM) while the ortho matrix uses x1-x0 / y1-y0 (a DIFFERENCE). This is only self-
 *    consistent when x0==0 && y0==0 — a latent quirk of this debug path, reproduced verbatim.
 *  - All positions carry a -0.5 half-pixel offset; vertex colors are 0xFFFFFFFF.
 *  - SetPixelShaderConstantFN / SetVertexShaderConstantFN pending masks (1<<63 and 3<<59) come from the
 *    extldi/sldi immediates; the decompiler rendered them as HIDWORD/__ROR4__ register puns. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rectangle2d.h"
#include "headers/rasterizer_render_target.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask1);
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
        const float *pConstantData, unsigned int Vector4fCount, unsigned __int64 PendingMask0);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, _D3DPRIMITIVETYPE primitive_type, unsigned int vertex_count,
        const void *vertex_data, unsigned int stride);

void rasterizer_render_target_test_target(int16_t target, const rectangle2d *bounds)
{
    if ( target == -1 )
        return;

    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_dyn_geom);

    D3DTexture *test_texture = (target >= 0 && target < 10) ? global_render_targets[target].texture : 0;
    ID3DXEffect_SetTexture(shader->effect, shader->texture[0], (struct D3DBaseTexture *)test_texture);

    unsigned int pass_count[4];
    ID3DXEffect_Begin(shader->effect, pass_count, 3);
    ID3DXEffect_BeginPass(shader->effect, 0);

    float pixel_constants[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
    D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 2, (unsigned __int64)1 << 63);

    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 2);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 2);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 0);
    D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, 0, 0);

    D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

    /* screen X from the rectangle's x-edges, screen Y from its y-edges; far corner is the SUM (see header) */
    float screen_x_low = (float)bounds->__s1.x0 - 0.5f;
    float screen_x_high = (float)(bounds->__s1.x0 + bounds->__s1.x1) - 0.5f;
    float screen_y_low = (float)bounds->__s1.y0 - 0.5f;
    float screen_y_high = (float)(bounds->__s1.y0 + bounds->__s1.y1) - 0.5f;

    dynamic_screen_vertex vertices[4];
    vertices[0].position = (real_point3d){ { screen_x_low, screen_y_low, 0.0f } };
    vertices[0].color = 0xFFFFFFFF;
    vertices[0].texcoord = (real_point2d){ { 0.0f, 0.0f } };
    vertices[1].position = (real_point3d){ { screen_x_high, screen_y_low, 0.0f } };
    vertices[1].color = 0xFFFFFFFF;
    vertices[1].texcoord = (real_point2d){ { 1.0f, 0.0f } };
    vertices[2].position = (real_point3d){ { screen_x_high, screen_y_high, 0.0f } };
    vertices[2].color = 0xFFFFFFFF;
    vertices[2].texcoord = (real_point2d){ { 1.0f, 1.0f } };
    vertices[3].position = (real_point3d){ { screen_x_low, screen_y_high, 0.0f } };
    vertices[3].color = 0xFFFFFFFF;
    vertices[3].texcoord = (real_point2d){ { 0.0f, 1.0f } };

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));

    /* screen-space orthographic view matrix (5 vec4 at register 0xD), built from the rectangle extents */
    float inv_x_extent = 1.0f / (float)(bounds->__s1.x1 - bounds->__s1.x0);
    float inv_y_extent = 1.0f / (float)(bounds->__s1.y1 - bounds->__s1.y0);
    float view_constants[20] = {
        (inv_x_extent * 2.0f), 0.0f, 0.0f, (-1.0f - inv_x_extent),
        0.0f, (inv_y_extent * -2.0f), 0.0f, (inv_y_extent + 1.0f),
        0.0f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 0.0f,
    };
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, view_constants, 5, (unsigned __int64)3 << 59);

    D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertices, sizeof(dynamic_screen_vertex));

    ID3DXEffect_EndPass(shader->effect);
    ID3DXEffect_End(shader->effect);
}
