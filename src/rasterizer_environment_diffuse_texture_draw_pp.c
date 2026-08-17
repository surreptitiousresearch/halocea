/* rasterizer_environment_diffuse_texture_draw_pp @0x8378A030 — pixel-processor variant of the environment
 * diffuse-texture debug draw (one of the swappable rasterizer_environment_diffuse_texture_draw targets, run
 * when the draw_environment_textures debug toggle is set). Selects the diffuse-texture effect shader (index
 * derived from three shader radiosity fields), binds four bitmaps into the effect (base + three detail/blend
 * maps at stages 0..3), builds the c10..c12 texture-transform constants — optionally scaling each axis by the
 * ratio of the base map's dimensions to each detail map's dimensions so all maps share the base map's UV
 * space — layers in the shader's tint/flags/colour scalars and the animated u/v scroll, selects the vertex
 * declaration/shader, and draws the dynamic triangles once per effect pass.
 *
 * Modeled on the rasterizer_environment_diffuse_light_draw_pp twin. Reconstructed from disassembly
 * (0x8378A030-0x8378A3B4): the decompiler emitted "local variable allocation has failed" and rendered the six
 * texture-dimension ratios as __int128/OVERLAPPED word-shuffles. Decoded from the fdivs block (0x8378A224-238):
 * each rasterizer_set_texture_for_effect returns a point2d {width@n[0], height@n[1]}; the ratios are base
 * width/height (stage 0) divided by each other stage's width/height. When the shader's ratio-enable flag
 * (word @0x6C bit 0) is clear, all six ratios are 1.0. The texture-transform constants are one contiguous
 * 12-float buffer whose [7]/[11] receive the animated scroll offsets; SetVertexShaderConstantFN garble is reg
 * 0xA, count 3, mask (uint64)3<<60. Shader scalar/index fields are the decompiler's 40-byte shader[N] type-pun
 * reads, transcribed here as raw byte offsets for clarity. */

#include <stdint.h>
#include "headers/shader_environment.h"
#include "headers/shader_environment_diffuse_rescale_flags.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/global_frame_parameters.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, double time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void rasterizer_environment_diffuse_texture_draw_pp(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    if ( !rasterizer_debug_options.draw_environment_textures )
        return;

    const shader_environment *environment_shader = (const shader_environment *)shader;
    const shader_environment_diffuse_properties *diffuse = &environment_shader->environment.diffuse;

    /* shader index = 3 * (3 * type + detail_map_function) + micro_detail_map_function + 5. */
    int16_t shader_type = environment_shader->environment.type;
    int16_t detail_map_function = diffuse->detail_map_function;
    int16_t micro_detail_map_function = diffuse->micro_detail_map_function;
    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(
            (int16_t)(3 * (3 * shader_type + detail_map_function) + micro_detail_map_function + 5));
    if ( !effect_shader || !effect_shader->effect )
        return;

    point2d *stage0_map = rasterizer_set_texture_for_effect(0, 0, 1,
            diffuse->base_map.index, shader_permutation_index, effect_shader);
    point2d *stage1_map = rasterizer_set_texture_for_effect(1, 0, 2,
            diffuse->primary_detail_map.index, shader_permutation_index, effect_shader);
    point2d *stage2_map = rasterizer_set_texture_for_effect(2, 0, 2,
            diffuse->secondary_detail_map.index, shader_permutation_index, effect_shader);
    point2d *stage3_map = rasterizer_set_texture_for_effect(3, 0, 2,
            diffuse->micro_detail_map.index, shader_permutation_index, effect_shader);

    float width0_over_width1 = 1.0f;
    float width0_over_width2 = 1.0f;
    float width0_over_width3 = 1.0f;
    float height0_over_height1 = 1.0f;
    float height0_over_height2 = 1.0f;
    float height0_over_height3 = 1.0f;
    if ( (diffuse->flags & (1u << _shader_environment_diffuse_rescale_detail_maps_bit)) != 0 )
    {
        float width0 = (float)stage0_map->n[0];
        float height0 = (float)stage0_map->n[1];
        width0_over_width1 = (width0 / (float)stage1_map->n[0]);
        width0_over_width2 = (width0 / (float)stage2_map->n[0]);
        width0_over_width3 = (width0 / (float)stage3_map->n[0]);
        height0_over_height1 = (height0 / (float)stage1_map->n[1]);
        height0_over_height2 = (height0 / (float)stage2_map->n[1]);
        height0_over_height3 = (height0 / (float)stage3_map->n[1]);
    }

    float primary_detail_map_scale = diffuse->primary_detail_map_scale;
    float secondary_detail_map_scale = diffuse->secondary_detail_map_scale;
    float micro_detail_map_scale = diffuse->micro_detail_map_scale;

    /* c10..c12: per-map texture-transform; [7]/[11] receive the animated u/v scroll offsets. */
    float texture_transform_constants[12];
    texture_transform_constants[0] = (primary_detail_map_scale * width0_over_width1);
    texture_transform_constants[1] = (primary_detail_map_scale * height0_over_height1);
    texture_transform_constants[2] = (secondary_detail_map_scale * width0_over_width2);
    texture_transform_constants[3] = (secondary_detail_map_scale * height0_over_height2);
    texture_transform_constants[4] = 1.0f;
    texture_transform_constants[5] = 0.0f;
    texture_transform_constants[6] = (micro_detail_map_scale * width0_over_width3);
    texture_transform_constants[7] = 0.0f;
    texture_transform_constants[8] = 0.0f;
    texture_transform_constants[9] = 1.0f;
    texture_transform_constants[10] = (micro_detail_map_scale * height0_over_height3);
    texture_transform_constants[11] = 0.0f;
    shader_environment_texture_animation_evaluate(shader, global_frame_parameters.game_time_sec,
                                                  &texture_transform_constants[7],
                                                  &texture_transform_constants[11]);
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, texture_transform_constants, 3,
                                        (uint64_t)3 << 60);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(0));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(effect_shader->vshader9));

    unsigned int pass_count[4];
    ID3DXEffect_Begin(effect_shader->effect, pass_count, 3);
    for ( unsigned int i = 0; i < pass_count[0]; ++i )
    {
        ID3DXEffect_BeginPass(effect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index, first_triangle_index,
                                                          triangle_count, vertex_buffer);
        ID3DXEffect_EndPass(effect_shader->effect);
    }
    ID3DXEffect_End(effect_shader->effect);
}
