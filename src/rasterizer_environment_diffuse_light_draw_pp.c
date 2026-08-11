/* rasterizer_environment_diffuse_light_draw_pp @0x837895B0 — the pixel-processor (post-2.0-shader) variant of
 * the environment diffuse dynamic-light draw. One of the swappable rasterizer_environment_diffuse_light_draw_
 * function targets. Runs when the diffuse-lights debug toggle is on. Selects the diffuse-light effect shader
 * (index 4), binds the base map at stage 0 (or none when the shader_environment bump_map_is_specular_mask
 * flag is set), uploads the bump
 * texture-transform constants (with animated u/v scroll offsets), sets the single diffuse light-colour constant
 * (c1), selects the vertex declaration/shader, then draws the dynamic triangles once per effect pass.
 *
 * Modeled on the specular sibling _rasterizer_environment_specular_light_draw. The decompiler's 40-byte
 * shader[N] type-puns are resolved to the shader_environment DB members (diffuse.material_color,
 * diffuse.bump_map.index, diffuse.runtime_bump_map_scale).
 * DEVIATION (FPR-shadow, disasm 0x8378965C-0x837896C4): the texture-transform block is one contiguous 12-float
 * buffer the decompiler split into two arrays and lost the evaluate output pointers to — evaluate writes the
 * u/v scroll offsets into elements [7] and [11]; the SetVertexShaderConstantFN garble is reg 0xA, count 3, mask
 * (uint64)3<<60. Unlike the specular variant there is NO constants null-guard here — the c1 SetVector is
 * unconditional (disasm 0x837896CC-0x83789700). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_environment.h"
#include "headers/shader_environment_flags.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/global_frame_parameters.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, float time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void rasterizer_environment_diffuse_light_draw_pp(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    const shader_environment *env = (const shader_environment *)shader;
    if ( rasterizer_debug_options.drawing_mode || !rasterizer_debug_options.draw_environment_diffuse_lights )
        return;

    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(_dxshader_environment_diffuse_lights);
    if ( !effect_shader || !effect_shader->effect )
        return;

    int bump_map_index = -1;
    /* the environment flags word is the shader_environment flags enum (DB $D2D05304...); its
     * bump_map_is_specular_mask bit gates the stage-0 bump-map binding here. */
    if ( (env->environment.flags & (1u << _shader_environment_bump_map_is_specular_mask_bit)) == 0 )
        bump_map_index = env->environment.diffuse.bump_map.index;
    rasterizer_set_texture_for_effect(0, 0, 3, bump_map_index, shader_permutation_index, effect_shader);

    /* c10..c12 bump texture-transform block; [7]/[11] receive the animated u/v scroll offsets. */
    float texture_transform_constants[12];
    texture_transform_constants[0] = env->environment.diffuse.runtime_bump_map_scale.n[0];
    texture_transform_constants[1] = env->environment.diffuse.runtime_bump_map_scale.n[1];
    texture_transform_constants[2] = 1.0f;
    texture_transform_constants[3] = 1.0f;
    texture_transform_constants[4] = 1.0f;
    texture_transform_constants[5] = 0.0f;
    texture_transform_constants[6] = 0.0f;
    texture_transform_constants[7] = 0.0f;
    texture_transform_constants[8] = 0.0f;
    texture_transform_constants[9] = 1.0f;
    texture_transform_constants[10] = 0.0f;
    texture_transform_constants[11] = 0.0f;
    shader_environment_texture_animation_evaluate(shader, global_frame_parameters.game_time_sec,
                                                  &texture_transform_constants[7],
                                                  &texture_transform_constants[11]);
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xA, texture_transform_constants, 3,
                                        (uint64_t)3 << 60);

    /* c1: diffuse material colour (r, g, b, 1). */
    ID3DXEffect *effect = effect_shader->effect;
    unsigned int *constants = effect_shader->constants;
    float vector[4];
    vector[0] = env->environment.diffuse.material_color.n[0];
    vector[1] = env->environment.diffuse.material_color.n[1];
    vector[2] = env->environment.diffuse.material_color.n[2];
    vector[3] = 1.0f;
    ID3DXEffect_SetVector(effect, constants[1], (const D3DXVECTOR4 *)vector);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(0));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_environment_diffuse_light));

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
