/* rasterizer_glass_draw_tint_pp @0x8378D978 — the pixel-processor variant of the glass "tint" draw pass. Binds
 * the glass base map at stage 0, selects the vertex declaration/shader for the group's primary vertex type,
 * uploads the base-map texture-transform constants scaled by the shader's tint colour (c10..c12), then
 * configures a three-stage fixed-function texture pipeline (modulate base by tint, combine, pass through) with
 * additive-into-framebuffer blending and alpha test, and finally draws the group. Stage 0's alpha op depends on
 * the group effect type (type 1 = modulate with the tint's alpha argument, else pass the base alpha).
 *
 * Modeled on the sibling rasterizer_environment_diffuse_light_draw_pp; the group's shader is the
 * shader_transparent_glass tag (the decompiler's 40-byte shader[N] byte-offset puns resolved to the DB
 * members: tint_map.index @0x70, tint_map_scale @0x60). D3DTSS_* stage-state selectors are
 * written as their numeric D3D9 values (corpus convention) with the symbolic name in a comment; values verified
 * against disasm 0x8378DAB0-0x8378DBAC. DEVIATION: for a primary vertex type other than 0/2/4 the original
 * reads an uninitialized stack slot for the vertex-shader variant (dead path for glass); reproduced as 0. The
 * SetVertexShaderConstantFN pending mask is (uint64)3<<60 (disasm 0x8378DA2C). */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/shader_transparent_glass.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/blam_data_globals.h"
#include "headers/_D3DTEXTURESTAGESTATETYPE.h"
#include "headers/render_model_effect_type.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"


#include "headers/point2d.h"
extern int16_t rasterizer_transparent_geometry_get_primary_vertex_type(const transparent_geometry_group *group);
extern point2d *rasterizer_set_texture(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
        const float *pConstantData, unsigned int Vector4fCount, uint64_t PendingMask0);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern void D3DDevice_SetRenderState_SrcBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_DestBlend(D3DDevice *device, unsigned int blend);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value);
extern void rasterizer_transparent_geometry_group_draw_internal(const transparent_geometry_group *group, uint8_t has_lightmap);

void rasterizer_glass_draw_tint_pp(const transparent_geometry_group *group)
{
    const shader_transparent_glass *glass = (const shader_transparent_glass *)group->shader;
    unsigned int primary_vertex_type = rasterizer_transparent_geometry_get_primary_vertex_type(group);

    int16_t vertex_shader_variant;
    if ( !primary_vertex_type || primary_vertex_type == _vsdecl_environment_lightmap )
        vertex_shader_variant = 0;
    else if ( primary_vertex_type == _vsdecl_model )
        vertex_shader_variant = 1;
    else
        vertex_shader_variant = 0;   /* original reads an uninitialized slot; dead path for glass vertex types */

    rasterizer_set_texture(0, 0, 1, glass->glass.tint_map.index, group->shader_permutation_index);
    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(primary_vertex_type));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(vertex_shader_variant + 55));

    /* c10..c12: tint-map texture transform, x/y scaled by the tint map scale */
    float texture_transform_constants[12];
    texture_transform_constants[0] = group->model_base_map_scale.n[0] * glass->glass.tint_map_scale;
    texture_transform_constants[1] = group->model_base_map_scale.n[1] * glass->glass.tint_map_scale;
    texture_transform_constants[2] = 1.0f;
    texture_transform_constants[3] = 1.0f;
    texture_transform_constants[4] = 0.0f;
    texture_transform_constants[5] = 0.0f;
    texture_transform_constants[6] = 0.0f;
    texture_transform_constants[7] = 0.0f;
    texture_transform_constants[8] = 0.0f;
    texture_transform_constants[9] = 1.0f;
    texture_transform_constants[10] = 0.0f;
    texture_transform_constants[11] = 0.0f;
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, texture_transform_constants, 3,
            (uint64_t)3 << 60);

    D3DDevice_SetPixelShader(global_d3d_device, nullptr);
    D3DDevice_SetRenderState_SrcBlend(global_d3d_device, 0);
    D3DDevice_SetRenderState_DestBlend(global_d3d_device, 4);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 1);

    SetTextureStageStateSmart(0, D3DTSS_COLOROP, 4);   /* modulate */
    SetTextureStageStateSmart(0, D3DTSS_COLORARG1, 2);
    SetTextureStageStateSmart(0, D3DTSS_COLORARG2, 3);

    unsigned int alpha_arg_value;
    unsigned int alpha_arg_state;
    if ( group->effect.type == _render_model_effect_type_active_camouflage )
    {
        SetTextureStageStateSmart(0, D3DTSS_ALPHAOP, 4);   /* modulate */
        SetTextureStageStateSmart(0, D3DTSS_ALPHAARG1, 0);
        alpha_arg_value = 3;
        alpha_arg_state = D3DTSS_ALPHAARG2;
    }
    else
    {
        SetTextureStageStateSmart(0, D3DTSS_ALPHAOP, 2);   /* selectarg1 */
        alpha_arg_value = 0;
        alpha_arg_state = D3DTSS_ALPHAARG1;
    }
    SetTextureStageStateSmart(0, alpha_arg_state, alpha_arg_value);

    SetTextureStageStateSmart(1, D3DTSS_COLOROP, 0x19);   /* multiplyadd */
    SetTextureStageStateSmart(1, D3DTSS_COLORARG1, 1);
    SetTextureStageStateSmart(1, D3DTSS_COLORARG2, 0x20);
    SetTextureStageStateSmart(1, D3DTSS_COLORARG0, 0x30);
    SetTextureStageStateSmart(1, D3DTSS_ALPHAOP, 2);      /* selectarg1 */
    SetTextureStageStateSmart(1, D3DTSS_ALPHAARG1, 1);

    SetTextureStageStateSmart(2, D3DTSS_COLOROP, 1);      /* disable */
    SetTextureStageStateSmart(2, D3DTSS_ALPHAOP, 1);      /* disable */

    rasterizer_transparent_geometry_group_draw_internal(group, 0);
}
