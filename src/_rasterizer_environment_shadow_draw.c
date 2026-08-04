/* _rasterizer_environment_shadow_draw @0x837A3DD0 — draws one batch of dynamic-geometry shadow-volume
 * triangles into the environment shadow render target. On the first call after
 * `_rasterizer_environment_shadow_begin` latched a new shadow (`shadow_setup` still false), this also does
 * the one-time-per-shadow setup: optionally convolves (blurs) the shadow buffer, binds it plus a linear
 * corner-fade ramp as stage 0/1 textures, pushes the render/blend/alpha-test state for shadow compositing,
 * uploads the shadow projection matrix (rescaled by the object's bounding radius, from the copy
 * `_rasterizer_environment_shadow_begin` stored) as 5 vertex shader constant vector4s, and uploads
 * `1 - shadow_color` as a pixel shader vector. It then restores the real render target once per shadow
 * (`shadow_restored`), sets stencil mode 2, and draws the triangle range through the shadow shader's effect
 * passes.
 *
 * DEVIATION: several D3D calls here show up as raw `m_Constants.Fetch[N].Texture`/`m_Pending.m_Mask` GPU
 * register pokes in the decompile, same as the already-resolved sibling `_rasterizer_model_begin`/
 * `_rasterizer_environment_*_begin` functions — restored to the equivalent `D3DDevice_SetSamplerState_*`
 * calls by matching the identical bit-shift/mask pattern. Two further decompiler misreads were caught by
 * checking `disasm_range` directly (this function does *not* carry the "local variable allocation failed"
 * warning, but still corrupted a couple of arguments through `__int128`/OVERLAPPED register-pun tricks):
 * `D3DDevice_SetSamplerState_MagFilter`'s third argument displayed as `DWORD1(v14)` is the literal constant
 * 1 (confirmed via the `li r5, 1 # Value` immediately preceding the `bl`), and the final
 * `D3DDevice_SetVertexShaderConstantFN`'s Vector4fCount/PendingMask0 (decompiler: `0x18000000u`/`0x82000000`)
 * are actually `5` and `(unsigned __int64)3 << 59` (traced from `li r6, 5` / `sldi r7, r7, 59` with `r7`
 * preloaded to 3) — the same encoding already established for this call in sibling shadow/reflection draw
 * functions. The projection-matrix constant buffer is modeled as `float constants[20]` (5 vector4s, matching
 * `Vector4fCount`) rather than the decompiler's oversized `float v33[38]` stack array — entries 20-37 are
 * never written or read, an artifact of stack packing with an unrelated neighboring local, not a real part
 * of the upload. Extends `game_globals_rasterizer_data` with the `linear_corner_fade` tag reference at
 * offset 0x40 (previously undifferentiated opaque padding). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_rgb_color.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"


typedef struct D3DVertexShader D3DVertexShader;
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_shadow_convolve(void);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void rasterizer_set_target(int16_t target, int16_t mipmap_index, uint32_t background_color, uint8_t clear, uint8_t zbuffer);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *pDevice, D3DVertexDeclaration *pDecl);
extern void D3DDevice_SetVertexShader(D3DDevice *pDevice, D3DVertexShader *pShader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *pDevice, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                unsigned __int64 PendingMask0);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

/* per-shadow scratch context, latched by _rasterizer_environment_shadow_begin */
extern float             rasterizer_shadow_bounding_radius;             /* object bounding radius */

void _rasterizer_environment_shadow_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    (void)shader;
    (void)shader_permutation_index;

    if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary || rasterizer_globals.render_targets_disabled)
        return;
    if (!rasterizer_debug_options.draw_environment_shadows)
        return;

    rasterizer_dx9_shader *dxeffect_shader = rasterizer_shader_select(_dxshader_environment_shadow);
    if (!dxeffect_shader || !dxeffect_shader->effect)
        return;

    if (!shadow_setup)
    {
        if (rasterizer_debug_options.shadow_convolution_enabled)
            rasterizer_shadow_convolve();

        rasterizer_set_target_as_texture_for_effect(
            0, rasterizer_debug_options.shadow_convolution_enabled ? 4 : 3, 0, dxeffect_shader);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);

        rasterizer_set_texture_direct_for_effect(1, global_rasterizer_data->linear_corner_fade.index, 0,
                                                 dxeffect_shader);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 1);

        D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xF);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 0);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, 5);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_AlphaRef(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_ZFunc(global_d3d_device, 2);
        D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);

        /* rescale the shadow-begin's cached projection matrix by 1/object_bounding_radius, splatting the
         * upper 3x3 into a compact 5-vector4 constant block matching the shadow vertex shader's layout */
        float oo_radius = 1.0f / rasterizer_shadow_bounding_radius;

        float constants[20];
        constants[0] = environment_shadow_projection_matrix.forward.n[0] * oo_radius * 0.5f;
        constants[1] = environment_shadow_projection_matrix.forward.n[1] * oo_radius * 0.5f;
        constants[2] = environment_shadow_projection_matrix.forward.n[2] * oo_radius * 0.5f;
        constants[3] = -((environment_shadow_projection_matrix.position.n[0] * environment_shadow_projection_matrix.forward.n[0]
                        + environment_shadow_projection_matrix.position.n[2] * environment_shadow_projection_matrix.forward.n[2]
                        + environment_shadow_projection_matrix.position.n[1] * environment_shadow_projection_matrix.forward.n[1]) * oo_radius - 1.0f) * 0.5f;
        constants[4]  = environment_shadow_projection_matrix.left.n[0] * oo_radius * -0.5f;
        constants[5]  = environment_shadow_projection_matrix.left.n[1] * oo_radius * -0.5f;
        constants[6]  = environment_shadow_projection_matrix.left.n[2] * oo_radius * -0.5f;
        constants[7] = ((environment_shadow_projection_matrix.position.n[0] * environment_shadow_projection_matrix.left.n[0]
                       + environment_shadow_projection_matrix.position.n[2] * environment_shadow_projection_matrix.left.n[2]
                       + environment_shadow_projection_matrix.position.n[1] * environment_shadow_projection_matrix.left.n[1]) * oo_radius + 1.0f) * 0.5f;
        constants[8]  = environment_shadow_projection_matrix.up.n[0] * (oo_radius * 0.25f);
        constants[9]  = environment_shadow_projection_matrix.up.n[1] * (oo_radius * 0.25f);
        constants[10] = environment_shadow_projection_matrix.up.n[2] * (oo_radius * 0.25f);
        constants[11] = -((environment_shadow_projection_matrix.position.n[0] * environment_shadow_projection_matrix.up.n[0]
                        + environment_shadow_projection_matrix.position.n[2] * environment_shadow_projection_matrix.up.n[2]
                        + environment_shadow_projection_matrix.position.n[1] * environment_shadow_projection_matrix.up.n[1]) * (oo_radius * 0.25f));
        constants[12] = -(environment_shadow_projection_matrix.up.n[0] * (oo_radius * 2.0f));
        constants[13] = -(environment_shadow_projection_matrix.up.n[1] * (oo_radius * 2.0f));
        constants[14] = -(environment_shadow_projection_matrix.up.n[2] * (oo_radius * 2.0f));
        constants[15] = (environment_shadow_projection_matrix.position.n[0] * environment_shadow_projection_matrix.up.n[0]
                       + environment_shadow_projection_matrix.position.n[2] * environment_shadow_projection_matrix.up.n[2]
                       + environment_shadow_projection_matrix.position.n[1] * environment_shadow_projection_matrix.up.n[1]) * (oo_radius * 2.0f);
        constants[16] = environment_shadow_projection_matrix.up.n[0];
        constants[17] = environment_shadow_projection_matrix.up.n[1];
        constants[18] = environment_shadow_projection_matrix.up.n[2];
        constants[19] = 0.0f;

        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, constants, 5, (unsigned __int64)3 << 59);

        ID3DXEffect *effect = dxeffect_shader->effect;
        unsigned int *effect_constants = dxeffect_shader->constants;

        float shadow_color_pixel[4];
        shadow_color_pixel[0] = 1.0f - local_shadow_color.n[0];
        shadow_color_pixel[1] = 1.0f - local_shadow_color.n[1];
        shadow_color_pixel[2] = 1.0f - local_shadow_color.n[2];
        shadow_color_pixel[3] = 1.0f;
        effect->lpVtbl->SetVector(effect, *effect_constants, (const D3DXVECTOR4 *)shadow_color_pixel);

        shadow_setup = 1;
    }

    if (!shadow_restored)
    {
        rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
        shadow_restored = 1;
    }

    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(vertex_buffer->type));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_environment_shadow));

    unsigned int pass_count;
    dxeffect_shader->effect->lpVtbl->Begin(dxeffect_shader->effect, &pass_count, 3);
    for (unsigned int i = 0; i < pass_count; ++i)
    {
        dxeffect_shader->effect->lpVtbl->BeginPass(dxeffect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index, first_triangle_index,
                                                          triangle_count, vertex_buffer);
        dxeffect_shader->effect->lpVtbl->EndPass(dxeffect_shader->effect);
    }
    dxeffect_shader->effect->lpVtbl->End(dxeffect_shader->effect);
}
