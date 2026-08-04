/* _rasterizer_environment_shadow_model_draw @0x837A3AF8 — draw one model's environment-shadow shader
 * (type 4) into the shadow-volume render target. Selects the shadow effect shader (46), sets cull mode
 * from the shader's two-sided flag, and (unless the shader's "no shadow map" flag is set) binds the
 * shader's diffuse bitmap to stage 0 wrapped/point-filtered, then uploads a self-illumination-derived
 * constant plus the diffuse UV animation transform (two vector4s written directly by
 * shader_texture_animation_evaluate) before drawing the model's static triangle/vertex buffers once per
 * effect pass.
 *
 * DEVIATION 1: kept the decompiler's raw 40-byte shader[N] type-pun indexing (as in the sibling
 * environment draw passes) rather than forcing every field onto the shader_environment struct — the
 * bitmap-group-index field at shader[4].base.radiosity.color.n[2] does not correspond to any named
 * shader_environment_diffuse_properties member at that byte offset (confirmed against the DB's own
 * types_members for that struct), so it is read as a raw LODWORD the same way the decompiler does.
 * DEVIATION 2: the second D3DDevice_SetVertexShaderConstantFN's Vector4fCount/PendingMask0 (decompiler:
 * 0x30000000u / unresolved v17) are recovered from disasm as the literal (3, (unsigned __int64)3 << 60)
 * — the same encoding used by the already-reversed sibling _rasterizer_environment_reflection_draw.
 * DEVIATION 3: u_transform_reference/v_transform_reference (decompiler args 9/10) are stack-spilled
 * pointers (real_vector4d[3] can't fit the 6 preceding float args in GPR shadow slots); traced from disasm
 * to &constants[4] and &constants[8] — the two vector4 slots shader_texture_animation_evaluate actually
 * writes, immediately followed by the SetVertexShaderConstantFN upload of all three vector4s starting at
 * constants[0]. The decompiler's static pre-fill of those slots is dead (overwritten by the call) and is
 * not reproduced. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_model.h"
#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/shader_texture_animation.h"
#include "headers/render_animation.h"
#include "headers/real_vector4d.h"
#include "headers/global_frame_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/hexrays_defs.h"
#include "headers/point2d.h"
#include "headers/rasterizer_target.h"
#include "headers/shader_type.h"
#include "headers/shader_model_flags.h"
#include "headers/blam_data_globals.h"


typedef struct D3DVertexShader D3DVertexShader;
#include "headers/render_animation.h"
#include "headers/point2d.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MipFilter_Inline(D3DDevice *pDevice, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *pDevice, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                unsigned __int64 PendingMask0);
extern void shader_texture_animation_evaluate(const shader_texture_animation *texture_animation, const render_animation *render_animation, float u_scale, float v_scale, float u_offset, float v_offset, float r_offset, float time_value, real_vector4d *u_transform_reference, real_vector4d *v_transform_reference);
extern void rasterizer_draw_static_triangles_static_vertices(const triangle_buffer *triangle_buffer, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *pDevice, D3DVertexDeclaration *pDecl);
extern void D3DDevice_SetVertexShader(D3DDevice *pDevice, D3DVertexShader *pShader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);

void _rasterizer_environment_shadow_model_draw(
        const shader          *shader,
        int16_t                shader_permutation_index,
        const triangle_buffer *triangle_buffer,
        const vertex_buffer   *vertex_buffer)
{
    if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary
     || rasterizer_globals.render_targets_disabled
     || !rasterizer_debug_options.draw_environment_shadows
     || shader->base.type != _shader_type_model)
        return;

    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_model_shadow);

    D3DDevice_SetRenderState_CullMode(global_d3d_device, (shader[1].base.radiosity.flags & (1u << _shader_model_two_sided_bit)) ? 0 : 6);

    if (!(shader[1].base.radiosity.flags & (1u << _shader_model_not_alpha_tested_bit)))
    {
        rasterizer_set_texture_for_effect(0, 0, 1, *(int *)&shader[4].base.radiosity.color.n[2],
                                          shader_permutation_index, effect_shader);
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 0, 0);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 0, 0);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 0, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 0, 1);
        D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 0, 1);
    }

    float self_illumination_color = shader[5].base.radiosity.color.n[2];
    float self_illumination_power = *(float *)&shader[5].base.type;

    /* c0xA: self-illumination intensity vector; c0xB/c0xC: diffuse UV animation transform (written below). */
    float constants[12];
    constants[0] = self_illumination_color;
    constants[1] = self_illumination_power * self_illumination_color;
    constants[2] = 1.0f;
    constants[3] = 1.0f;

    float u_scale = local_parameters_0->base_map_scale.n[0] * *(float *)&shader[3].base.type;
    float v_scale = local_parameters_0->base_map_scale.n[1] * *(float *)&shader[4].base.radiosity.flags;

    shader_texture_animation_evaluate(
            &((const shader_model *)shader)->model.animation,
            &local_parameters_0->animation,
            u_scale, v_scale, 0.0f, 0.0f, 0.0f,
            (float)global_frame_parameters.game_time_sec,
            (real_vector4d *)&constants[4],
            (real_vector4d *)&constants[8]);

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, constants, 3, (unsigned __int64)3 << 60);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(vertex_buffer->type));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_model_shadow));

    unsigned int pass_count;
    effect_shader->effect->lpVtbl->Begin(effect_shader->effect, &pass_count, 3);
    for (unsigned int i = 0; i < pass_count; ++i)
    {
        effect_shader->effect->lpVtbl->BeginPass(effect_shader->effect, i);
        rasterizer_draw_static_triangles_static_vertices(triangle_buffer, 0, triangle_buffer->count, vertex_buffer);
        effect_shader->effect->lpVtbl->EndPass(effect_shader->effect);
    }
    effect_shader->effect->lpVtbl->End(effect_shader->effect);
}
