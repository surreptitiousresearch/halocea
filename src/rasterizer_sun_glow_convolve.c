/* rasterizer_sun_glow_convolve @0x83801830 — post-process blur that ping-pongs the sun-glow render
 * target through `passes` additive convolution passes (effect 76). Each pass binds the current source
 * target as a texture on all 4 sampler stages, draws a full-viewport quad into the alternate target with
 * additive blending, and halves the contribution weight after the first pass. Returns the target id that
 * holds the final result (primary if `passes` is even, secondary if odd).
 *
 * Sibling of rasterizer_shadow_convolve.c — same shader-select / vdecl / vshader / DrawVerticesUP shape.
 *
 * DEVIATION (catalog class 5/9, "local variable allocation failed" register puns): the per-stage sampler
 * setup was emitted as a raw m_Constants.Fetch[stage].Texture / m_Pending.m_Mask[3] poke — clearing the
 * 0x01800000 field of dword[3] and ORing the per-stage dirty bit (0x80000000 >> stage). Disasm shows the
 * accumulator is the constant rotl64(1,63)=0x8000000000000000 (the decompiler aliased it with the target
 * id via `HIDWORD(v12)=v15`); the whole poke is the established `SeparateZFilterEnable = 0` sampler call
 * (see _rasterizer_decals_begin.c, which pokes the same field/mask for the enabled case).
 *
 * DEVIATION: D3DDevice_SetVertexShaderConstantFN's Vector4fCount/PendingMask0 render as garbage in the
 * decompile; disasm settles them to 8 and (uint64)7<<58 (the 8-vector4 texanim matrix upload). */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"

extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetSamplerState_SeparateZFilterEnable(D3DDevice *device, unsigned int sampler,
        unsigned int value);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int value);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_BlendOp(D3DDevice *device, unsigned int op);
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

int16_t rasterizer_sun_glow_convolve(int16_t primary_target, int16_t secondary_target, int16_t passes)
{
    rasterizer_dx9_shader *shader = rasterizer_shader_select(_dxshader_sun_glow_convolve);
    if (shader && shader->effect && passes > 0)
    {
        D3DDevice_SetRenderState_CullMode(global_d3d_device, 6);
        D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 7);
        D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 1);
        D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 0xA);
        D3DDevice_SetRenderState_DestBlend(global_d3d_device, 0);
        D3DDevice_SetRenderState_BlendOp(global_d3d_device, 0);
        D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
        D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);

        D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
        D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(0));
        D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vsh_constants__texanim, 8,
                (uint64_t)7 << 58);

        for (int16_t pass = 0; pass < passes; pass++)
        {
            int16_t source_target = (pass & 1) == 0 ? primary_target : secondary_target;
            int16_t destination_target = (pass & 1) == 0 ? secondary_target : primary_target;

            for (int16_t stage = 0; stage < 4; stage++)
            {
                rasterizer_set_target_as_texture_for_effect(stage, source_target, 0, shader);
                D3DDevice_SetSamplerState_SeparateZFilterEnable(global_d3d_device, stage, 0);
            }

            rasterizer_set_target(destination_target, 0, 0, 0, 0);

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

            /* First pass contributes at full weight, subsequent passes at half (additive accumulation). */
            float weight = pass <= 0 ? 1.0f : 0.5f;
            float convolve_weight[4] = { weight, weight, weight, weight };
            ID3DXEffect_SetVector(shader->effect, *shader->constants, (const D3DXVECTOR4 *)convolve_weight);

            unsigned int pass_count[4];
            ID3DXEffect_Begin(shader->effect, pass_count, 3);
            ID3DXEffect_BeginPass(shader->effect, 1);
            D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4, vertex_data_3,
                    sizeof(dynamic_screen_vertex));
            ID3DXEffect_EndPass(shader->effect);
            ID3DXEffect_End(shader->effect);
        }

        rasterizer_set_target(global_window_parameters.rasterizer_target, 0, 0, 0, 1);
    }

    return (passes & 1) == 0 ? primary_target : secondary_target;
}
