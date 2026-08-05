/* rasterizer_glass_draw_diffuse_pp @0x8378E410 — the pixel-processor "diffuse" draw pass for a glass
 * transparent-geometry group. Selects the glass diffuse effect shader (109), picks the vertex
 * declaration/shader for the group's primary vertex type (a lightmapped decl when the group carries a
 * lightmap), uploads the base-map texture-transform constants (c10..c12) scaled by the shader's tint/detail
 * colours, binds the base map (stage 0) and a second map (stage 1) into the effect, then — when a lightmap is
 * present — binds it (stage 2) and configures its sampler. Additive-into-framebuffer blend + alpha test, then
 * draws the group once through the effect pass selected by lightmap presence.
 *
 * DEVIATION 1: the inline sampler-state register pokes (global_d3d_device->m_Constants.Fetch[N].Texture writes
 * with rlwinm/__ROL4__ masks) are the compiler's inlining of the standard SetSamplerState helpers; decoded via
 * disasm (0x8378E5B4-0x8378E6CC): stage 1 = AddressU 0 / AddressV 0 / MagFilter 1 / MinFilter 1 / MipFilter 1
 * (rlwinm ...,22,18 clears the AddressU field, ...,19,15 clears AddressV; MagFilter Value = li r5,1); stage 2 =
 * AddressU 2 / AddressV 2 / MagFilter 1 / MinFilter 1 / MipFilter 1. Matches the _rasterizer_environment_
 * shadow_model_draw sibling's decoding.
 * DEVIATION 2: SetVertexShaderConstantFN count/mask (decompiler v13 garbage) recovered from disasm as
 * (3, (unsigned __int64)3 << 60) (li r6,3; li r7,3; sldi r7,r7,60). MagFilter's stage-1 Value (decompiler
 * HIDWORD garbage) is 1.
 * DEVIATION 3: for a primary vertex type other than 0/2/4 the original reads an uninitialized stack slot for
 * the vertex-shader variant (dead path for glass); reproduced as 0. The decompiler's 40-byte shader[N]
 * type-pun reads are resolved to the shader_transparent_glass DB members (diffuse_map/diffuse_detail_map
 * indices + scales). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/shader_transparent_glass.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/bitmap_data.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/bitmap_data.h"
#include "headers/point2d.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetSamplerState_AddressU_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_AddressV_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MagFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MinFilter(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetSamplerState_MipFilter_Inline(D3DDevice *device, unsigned int Sampler, unsigned int Value);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);

void rasterizer_glass_draw_diffuse_pp(const transparent_geometry_group *group)
{
    const shader_transparent_glass *glass = (const shader_transparent_glass *)group->shader;
    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_transparent_glass_diffuse);
    if ( !effect_shader || !effect_shader->effect )
        return;

    int16_t primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);
    unsigned int pass_index = 0;

    int16_t vertex_shader_variant;
    if ( !primary_vertex_type || primary_vertex_type == _rasterizer_vertex_type_environment_lightmap_uncompressed )
    {
        vertex_shader_variant = 0;
        D3DDevice_SetVertexDeclaration(global_d3d_device,
                rasterizer_dx9_shaders_vdecl9_get(group->lightmap ? 2 : 0));
    }
    else if ( primary_vertex_type == _rasterizer_vertex_type_model_uncompressed )
    {
        vertex_shader_variant = 1;
        D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_model));
    }
    else
    {
        vertex_shader_variant = 0;   /* original reads an uninitialized slot; dead path for glass vertex types */
    }
    D3DDevice_SetVertexShader(global_d3d_device,
            rasterizer_dx9_shaders_vshader9_get(vertex_shader_variant + 48));

    /* c10..c12: base-map texture transform (x/y scaled by the diffuse map scale, second pair by the
     * diffuse detail map scale) */
    float base_map_transform_constants[12];
    base_map_transform_constants[0] =
            group->model_base_map_scale.n[0] * glass->glass.diffuse_map_scale;
    base_map_transform_constants[1] =
            group->model_base_map_scale.n[1] * glass->glass.diffuse_map_scale;
    base_map_transform_constants[2] =
            group->model_base_map_scale.n[0] * glass->glass.diffuse_detail_map_scale;
    base_map_transform_constants[3] =
            group->model_base_map_scale.n[1] * glass->glass.diffuse_detail_map_scale;
    base_map_transform_constants[4] = 0.0f;
    base_map_transform_constants[5] = 0.0f;
    base_map_transform_constants[6] = 0.0f;
    base_map_transform_constants[7] = 0.0f;
    base_map_transform_constants[8] = 0.0f;
    base_map_transform_constants[9] = 1.0f;
    base_map_transform_constants[10] = 0.0f;
    base_map_transform_constants[11] = 0.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, base_map_transform_constants, 3,
            (uint64_t)3 << 60);

    rasterizer_set_texture_for_effect(0, 0, 1, glass->glass.diffuse_map.index,
            group->shader_permutation_index, effect_shader);
    rasterizer_set_texture_for_effect(1, 0, 2, glass->glass.diffuse_detail_map.index,
            group->shader_permutation_index, effect_shader);

    D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 1, 0);
    D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 1, 1);
    D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 1, 1);

    if ( group->lightmap )
    {
        rasterizer_set_texture_bitmap_data_for_effect(2, (bitmap_data *)group->lightmap, effect_shader); /* lazy hw-format mutation: drop const view */
        D3DDevice_SetSamplerState_AddressU_Inline(global_d3d_device, 2, 2);
        D3DDevice_SetSamplerState_AddressV_Inline(global_d3d_device, 2, 2);
        D3DDevice_SetSamplerState_MagFilter(global_d3d_device, 2, 1);
        D3DDevice_SetSamplerState_MinFilter(global_d3d_device, 2, 1);
        pass_index = 1;
        D3DDevice_SetSamplerState_MipFilter_Inline(global_d3d_device, 2, 1);
    }

    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 6);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, 7);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);

    unsigned int passes[4];
    ID3DXEffect_Begin(effect_shader->effect, passes, 3);
    ID3DXEffect_BeginPass(effect_shader->effect, pass_index);
    rasterizer_transparent_geometry_group_draw_internal(group, group->lightmap != nullptr);
    ID3DXEffect_EndPass(effect_shader->effect);
    ID3DXEffect_End(effect_shader->effect);
}
