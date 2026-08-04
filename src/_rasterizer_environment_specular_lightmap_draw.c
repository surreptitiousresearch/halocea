/* _rasterizer_environment_specular_lightmap_draw @0x837A5D08 — draw the per-surface specular-from-lightmap
 * pass for one environment-shader surface batch. Runs only when the specular-lightmap debug toggle is on,
 * real-time lightmaps are present, the shader has a positive specular brightness, and the shader's specular
 * flags request lightmap specular (bit 2). Selects one of two effect shaders (42/43) by whether the shader
 * uses the colored-lightmap path (environment flag bit 1), binds the bump map (stage 0), the specular
 * lightmap (stage 1, or null when absent), and the normalization cube map (stages 2/3), uploads the bump
 * texture-transform constants (registers c10..c12, with animated u/v scroll) and the specular tint/brightness
 * pixel constants (registers c0..c3), then draws the dynamic triangles once per effect pass.
 *
 * DEVIATION 1: shader pointer retyped from the decompiler's `_shader *` (shader[N]40-byte indexing) to
 * shader_environment* with named members. DEVIATION 2: canonical 6-arg draw-callback signature (the
 * permutation index is an __int16, not the decompiler's widened forms). DEVIATION 3: the texture-transform
 * constants the decompiler split into v35[7]+v36[5] are one contiguous 12-float block uploaded as 3 vector4s;
 * the evaluate call writes the u/v scroll offsets into elements [7] and [11]. */

#include <stdint.h>
#include "headers/shader_environment.h"
#include "headers/shader_environment_flags.h"
#include "headers/shader_environment_specular_flags.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/bitmap_data.h"
#include "headers/global_frame_parameters.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"

#include "headers/bitmap_data.h"
#include "headers/point2d.h"
extern bitmap_data *global_specular_lightmap;   /* DB: global_specular_lightmap */

extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);
extern void D3DDevice_SetTexture(D3DDevice *device, unsigned int Sampler, struct D3DBaseTexture *texture,
                                 unsigned __int64 fetch_constant);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, float time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                unsigned __int64 PendingMask0);
extern void D3DDevice_SetPixelShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                               const float *pConstantData, unsigned int Vector4fCount,
                                               unsigned __int64 PendingMask1);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices2(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer0, const vertex_buffer *vertex_buffer1);

void _rasterizer_environment_specular_lightmap_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    const shader_environment *shader_env = (const shader_environment *)shader;

    if (rasterizer_debug_options.drawing_mode
        || !rasterizer_debug_options.draw_environment_specular_lightmaps
        || rasterizer_globals.lightmap_mode
        || local_lightmap_has_no_data
        || shader_env->environment.specular.brightness <= 0.0f
        || (shader_env->environment.specular.flags & (1u << _shader_environment_specular_lightmap_bit)) == 0)
        return;

    __int16 effect_index = (shader_env->environment.flags & (1u << _shader_environment_bump_map_is_specular_mask_bit))
                               ? _dxshader_environment_specular_lightmap_flat
                               : _dxshader_environment_specular_lightmap_bumped;
    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(effect_index);
    if (!effect_shader || !effect_shader->effect)
        return;

    float lightmap_specular_exponent = (shader_env->environment.specular.flags & (1u << _shader_environment_specular_overbright_bit)) ? 4.0f : 2.0f;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment_lightmap));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(effect_shader->vshader9));

    rasterizer_set_texture_for_effect(0, 0, 3, shader_env->environment.diffuse.bump_map.index,
                                      shader_permutation_index, effect_shader);
    if (local_lightmap_has_no_data)
        D3DDevice_SetTexture(global_d3d_device, 1, NULL, 0x40000000);
    else
        rasterizer_set_texture_bitmap_data_for_effect(1, global_specular_lightmap, effect_shader);
    rasterizer_set_texture_direct_for_effect(2, global_rasterizer_data->vector_normalization.index, 0,
                                             effect_shader);
    rasterizer_set_texture_direct_for_effect(3, global_rasterizer_data->vector_normalization.index, 0,
                                             effect_shader);

    /* c10..c12 texture-transform block; evaluate scrolls the u/v offsets into [7] and [11]. */
    float texture_transform_constants[12];
    texture_transform_constants[0] = shader_env->environment.diffuse.runtime_bump_map_scale.__s1.i;
    texture_transform_constants[1] = shader_env->environment.diffuse.runtime_bump_map_scale.__s1.j;
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
                                        (unsigned __int64)3 << 60);

    /* c0..c3 specular tint constants: brightness, perpendicular color, parallel color, exponent. */
    float pixel_constants[16];
    pixel_constants[0] = shader_env->environment.specular.brightness;
    pixel_constants[1] = shader_env->environment.specular.brightness;
    pixel_constants[2] = shader_env->environment.specular.brightness;
    pixel_constants[3] = shader_env->environment.specular.brightness;
    pixel_constants[4] = shader_env->environment.specular.view_perpendicular_color.__s1.red;
    pixel_constants[5] = shader_env->environment.specular.view_perpendicular_color.__s1.green;
    pixel_constants[6] = shader_env->environment.specular.view_perpendicular_color.__s1.blue;
    pixel_constants[7] = 1.0f;
    pixel_constants[8] = shader_env->environment.specular.view_parallel_color.__s1.red;
    pixel_constants[9] = shader_env->environment.specular.view_parallel_color.__s1.green;
    pixel_constants[10] = shader_env->environment.specular.view_parallel_color.__s1.blue;
    pixel_constants[11] = 1.0f;
    pixel_constants[12] = lightmap_specular_exponent;   /* FAITHFUL QUIRK: c3 (exponent) is written but the */
    pixel_constants[13] = lightmap_specular_exponent;   /* upload count below is 3, so registers c0..c2 only */
    pixel_constants[14] = lightmap_specular_exponent;   /* are sent — these four writes are not uploaded.    */
    pixel_constants[15] = lightmap_specular_exponent;
    D3DDevice_SetPixelShaderConstantFN(global_d3d_device, 0, pixel_constants, 3, 0x8000000000000000ULL);

    unsigned int pass_count[4];
    effect_shader->effect->lpVtbl->Begin(effect_shader->effect, pass_count, 3);
    for (unsigned int i = 0; i < pass_count[0]; ++i)
    {
        effect_shader->effect->lpVtbl->BeginPass(effect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices2(dynamic_triangle_buffer_index, first_triangle_index,
                                                           triangle_count, vertex_buffer, vertex_buffer + 1);
        effect_shader->effect->lpVtbl->EndPass(effect_shader->effect);
    }
    effect_shader->effect->lpVtbl->End(effect_shader->effect);
}
