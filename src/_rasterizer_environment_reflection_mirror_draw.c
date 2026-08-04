/* _rasterizer_environment_reflection_mirror_draw @0x837A7280 — draw the planar-mirror reflection pass for one
 * environment surface batch. Only runs when the mirror debug toggle is on, the window actually has a mirror,
 * and we are rendering to the main framebuffer. Selects a mirror effect shader by the reflection mode and the
 * shader's colored/flat-lightmap flag, binds the bump map (stage 0, or skips it for an untextured mirror),
 * the normalization cube map, and the mirror render target as a texture, uploads the bump texture-transform
 * constants (with a 320x240 mirror projection scale baked in), the camera-forward / colored-lightmap vector,
 * and the reflection tint/brightness constants, then draws the dynamic triangles once per effect pass.
 *
 * DEVIATION 1: shader pointer retyped from the decompiler's `_shader *` (shader[N] 40-byte indexing) to
 * shader_environment* with named members. DEVIATION 2: canonical 6-arg draw-callback signature.
 * DEVIATION 3: the texture-transform constants the decompiler split into v43[7]+v44[5] are one contiguous
 * 12-float block uploaded as 3 vector4s; evaluate writes the u/v scroll offsets into elements [7] and [11].
 * FAITHFUL QUIRK: the reflection-mode == 3 branch selects the effect index from the high half-word of the
 * still-uninitialized effect pass-count buffer (a latent bug in the original; reproduced verbatim). */

#include <stdint.h>
#include "headers/shader_environment.h"
#include "headers/shader_environment_flags.h"
#include "headers/shader_environment_reflection_type.h"
#include "headers/shader_environment_reflection_mirror_flags.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/global_frame_parameters.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void rasterizer_set_target_as_texture_for_effect(int16_t stage, int16_t target, int16_t max_mipmap, rasterizer_dx9_shader *dxeffect_shader);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, float time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                unsigned __int64 PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void _rasterizer_environment_reflection_mirror_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    const shader_environment *shader_env = (const shader_environment *)shader;

    if (rasterizer_debug_options.drawing_mode
        || !rasterizer_debug_options.draw_environment_reflection_mirrors
        || !global_window_parameters.has_mirror
        || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary)
        return;

    int reflection_mode = shader_env->environment.reflection.type;
    if (reflection_mode == _shader_environment_reflection_type_bumped
        || reflection_mode == _shader_environment_reflection_type_radiosity)
    {
        if (shader_env->environment.flags & (1u << _shader_environment_bump_map_is_specular_mask_bit))
            reflection_mode = _shader_environment_reflection_type_flat;
        if (shader_env->environment.diffuse.bump_map.index == -1)
            reflection_mode = _shader_environment_reflection_type_flat;
    }

    if (!(shader_env->environment.reflection.flags & (1u << _shader_environment_reflection_mirror_bit))
        || !(shader_env->environment.reflection.view_perpendicular_brightness > 0.0f
             || shader_env->environment.reflection.view_parallel_brightness > 0.0f))
        return;

    unsigned int pass_count[4];
    __int16 effect_index;
    if (reflection_mode == _shader_environment_reflection_type_bumped)
        effect_index = _dxshader_environment_reflection_mirror_bumped;
    else if (reflection_mode == _shader_environment_reflection_type_flat)
        effect_index = (shader_env->environment.flags & (1u << _shader_environment_bump_map_is_specular_mask_bit))
                           ? _dxshader_environment_reflection_mirror_flat_specular
                           : _dxshader_environment_reflection_mirror_flat;
    else if ((unsigned int)reflection_mode >= NUMBER_OF_SHADER_ENVIRONMENT_REFLECTION_TYPES)
        effect_index = (__int16)(pass_count[0] >> 16); /* FAITHFUL QUIRK: reads uninitialized stack */
    else /* _shader_environment_reflection_type_radiosity */
        effect_index = _dxshader_environment_reflection_mirror_bumped;

    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(effect_index);
    if (!effect_shader || !effect_shader->effect)
        return;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(effect_shader->vshader9));

    int bump_map_index = shader_env->environment.diffuse.bump_map.index;
    __int16 normalization_stage;
    if (bump_map_index == -1)
    {
        normalization_stage = 2;
    }
    else
    {
        rasterizer_set_texture_for_effect(0, 0, 3, bump_map_index, shader_permutation_index, effect_shader);
        normalization_stage = 1;
    }
    rasterizer_set_texture_direct_for_effect(normalization_stage,
                                             global_rasterizer_data->vector_normalization.index, 0,
                                             effect_shader);
    rasterizer_set_target_as_texture_for_effect(3, 2, 0, effect_shader);

    /* c10..c12 texture-transform block; [2]/[3] carry the 320x240 mirror projection scale. */
    float texture_transform_constants[12];
    texture_transform_constants[0] = shader_env->environment.diffuse.runtime_bump_map_scale.__s1.i;
    texture_transform_constants[1] = shader_env->environment.diffuse.runtime_bump_map_scale.__s1.j;
    texture_transform_constants[2] = 320.0f;
    texture_transform_constants[3] = 240.0f;
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

    /* c0: camera forward remapped to [-1,1] (xyz) + colored-lightmap flag (w). */
    float mirror_constants[4];
    float forward_x = -(global_window_parameters.camera.forward.n[0] * 0.5f - 0.5f);
    forward_x = forward_x < 0.0f ? 0.0f : (forward_x > 1.0f ? 1.0f : forward_x);
    mirror_constants[0] = forward_x * 2.0f - 1.0f;
    float forward_y = -(global_window_parameters.camera.forward.n[1] * 0.5f - 0.5f);
    forward_y = forward_y < 0.0f ? 0.0f : (forward_y > 1.0f ? 1.0f : forward_y);
    mirror_constants[1] = forward_y * 2.0f - 1.0f;
    float forward_z = -(global_window_parameters.camera.forward.n[2] * 0.5f - 0.5f);
    forward_z = forward_z < 0.0f ? 0.0f : (forward_z > 1.0f ? 1.0f : forward_z);
    mirror_constants[2] = forward_z * 2.0f - 1.0f;
    mirror_constants[3] = (shader_env->environment.flags & (1u << _shader_environment_bump_map_is_specular_mask_bit)) ? -1.0f : 0.0f;

    /* c1/c2: reflection tint = specular perpendicular/parallel colors with reflection brightness in w. */
    unsigned int *constants = effect_shader->constants;
    float reflection_perpendicular[4];
    reflection_perpendicular[0] = shader_env->environment.specular.view_perpendicular_color.__s1.red;
    reflection_perpendicular[1] = shader_env->environment.specular.view_perpendicular_color.__s1.green;
    reflection_perpendicular[2] = shader_env->environment.specular.view_perpendicular_color.__s1.blue;
    reflection_perpendicular[3] = shader_env->environment.reflection.view_perpendicular_brightness;
    float reflection_parallel[4];
    reflection_parallel[0] = shader_env->environment.specular.view_parallel_color.__s1.red;
    reflection_parallel[1] = shader_env->environment.specular.view_parallel_color.__s1.green;
    reflection_parallel[2] = shader_env->environment.specular.view_parallel_color.__s1.blue;
    reflection_parallel[3] = shader_env->environment.reflection.view_parallel_brightness;

    if (constants)
    {
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, *constants,
                                         (const D3DXVECTOR4 *)mirror_constants);
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[1],
                                         (const D3DXVECTOR4 *)reflection_perpendicular);
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[2],
                                         (const D3DXVECTOR4 *)reflection_parallel);
    }

    effect_shader->effect->lpVtbl->Begin(effect_shader->effect, pass_count, 3);
    for (unsigned int i = 0; i < pass_count[0]; ++i)
    {
        effect_shader->effect->lpVtbl->BeginPass(effect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index, first_triangle_index,
                                                          triangle_count, vertex_buffer);
        effect_shader->effect->lpVtbl->EndPass(effect_shader->effect);
    }
    effect_shader->effect->lpVtbl->End(effect_shader->effect);
}
