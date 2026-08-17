/* _rasterizer_environment_reflection_lightmap_mask_draw @0x837A67B8 — draw the per-surface reflection-mask
 * pass for one environment-shader surface batch. The mask attenuates the cube-map reflection by the shader's
 * lightmap brightness, so it only runs when both the reflection and the reflection-lightmap-mask debug toggles
 * are on, real-time lightmaps are present, and the shader actually reflects (a non-zero perpendicular or
 * parallel reflection brightness) while its lightmap brightness scale leaves room to attenuate (< 1).
 *
 * It uploads the diffuse map's runtime bump-scale as a texture-transform constant block (registers c10..c12),
 * lets shader_environment_texture_animation_evaluate scroll the u/v offsets into that block, hands the
 * reflection lightmap-brightness scale to the effect as a vector parameter, binds the screen-space vertex
 * decl/shader, and draws the dynamic triangles once per effect pass.
 *
 * DEVIATION 1: the decompiler typed the shader pointer as `_shader *` and reached the environment fields by
 * indexing shader[N] (40-byte stride); recovered the real shader_environment layout and use named members.
 * DEVIATION 2: the canonical draw-callback signature is the 6-arg form (the thunk establishes it); the
 * decompiler widened the __int16 permutation index to a float* and invented a trailing long-double argument
 * from the FPR-shadowed game_time_sec.
 * DEVIATION 3: the 12-float texture-transform block the decompiler split into v22[7]+v23[5] is one contiguous
 * constant buffer uploaded as 3 vector4s; expressed here as a single array (the evaluate call writes the u/v
 * scroll offsets into elements [7] and [11]). */

#include <stdint.h>
#include "headers/shader_environment.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/global_frame_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


extern void shader_environment_texture_animation_evaluate(const struct shader *shader, double time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices2(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer0, const vertex_buffer *vertex_buffer1);

void _rasterizer_environment_reflection_lightmap_mask_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    const shader_environment *shader_env = (const shader_environment *)shader;

    if (rasterizer_debug_options.drawing_mode
        || !rasterizer_debug_options.draw_environment_reflection_lightmap_masks
        || !rasterizer_debug_options.draw_environment_reflections
        || rasterizer_globals.lightmap_mode
        || local_lightmap_has_no_data_0)
        return;

    if (!(shader_env->environment.reflection.view_perpendicular_brightness > 0.0f
          || shader_env->environment.reflection.view_parallel_brightness > 0.0f)
        || shader_env->environment.reflection.lightmap_brightness_scale >= 1.0f)
        return;

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
                                        (uint64_t)3 << 60);

    if (!dxeffect_shader)
        return;
    ID3DXEffect *effect = dxeffect_shader->effect;
    if (!effect)
        return;

    unsigned int *constants = dxeffect_shader->constants;
    float lightmap_brightness_vector[4];
    lightmap_brightness_vector[0] = shader_env->environment.reflection.lightmap_brightness_scale;
    lightmap_brightness_vector[1] = shader_env->environment.reflection.lightmap_brightness_scale;
    lightmap_brightness_vector[2] = shader_env->environment.reflection.lightmap_brightness_scale;
    lightmap_brightness_vector[3] = shader_env->environment.reflection.lightmap_brightness_scale;
    effect->lpVtbl->SetVector(effect, *constants, (const D3DXVECTOR4 *)lightmap_brightness_vector);

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment_lightmap));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(dxeffect_shader->vshader9));

    unsigned int pass_count[4];
    dxeffect_shader->effect->lpVtbl->Begin(dxeffect_shader->effect, pass_count, 3);
    for (unsigned int i = 0; i < pass_count[0]; ++i)
    {
        dxeffect_shader->effect->lpVtbl->BeginPass(dxeffect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices2(dynamic_triangle_buffer_index, first_triangle_index,
                                                           triangle_count, vertex_buffer, vertex_buffer + 1);
        dxeffect_shader->effect->lpVtbl->EndPass(dxeffect_shader->effect);
    }
    dxeffect_shader->effect->lpVtbl->End(dxeffect_shader->effect);
}
