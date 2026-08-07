/* _rasterizer_environment_specular_light_draw 0x837A4F50 — draw the environment specular dynamic-light pass for
 * one surface batch. Runs when the specular-lights debug toggle is on, the shader's specular brightness is
 * positive, and the global specular light brightness is positive. Selects a specular-light effect shader by the
 * shader's colored-lightmap flag, binds the bump map (stage 0), the distance-attenuation / spot gel map
 * (stage 1, direct or spot variant), and the normalization cube map (stages 2/3), uploads the point-light
 * vertex constants and the bump texture-transform constants, sets the per-light specular brightness and
 * view-perpendicular/view-parallel colour constants, then draws the dynamic triangles once per effect pass.
 *
 * DEVIATION 1: shader pointer retyped from the decompiler's 40-byte shader[N] type-pun indexing to
 * shader_environment* with named members (diffuse.bump_map/runtime_bump_map_scale, specular.flags/brightness/
 * view_perpendicular_color/view_parallel_color) — every offset DB types_members verified.
 * DEVIATION 2: the two D3DDevice_SetVertexShaderConstantFN count/mask pairs the decompiler garbled are, per the
 * disassembly, reg 0xD count 5 mask (uint64)3<<59 (point-light constants) and reg 0xA count 3 mask
 * (uint64)3<<60 (texture transform). The texture-transform block is one contiguous 12-float buffer the
 * decompiler split into two arrays; evaluate writes the u/v scroll offsets into elements [7] and [11].
 * DEVIATION 3: canonical 6-arg draw signature; the decompiler reuses one 4-float scratch for all four SetVector
 * uploads (each copied by SetVector before the next fill), expressed here as one reused buffer. */

#include <stdint.h>
#include "headers/shader_environment.h"
#include "headers/shader_environment_flags.h"
#include "headers/shader_environment_specular_flags.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_environment_light_globals.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/global_frame_parameters.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, double time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void _rasterizer_environment_specular_light_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    const shader_environment *shader_env = (const shader_environment *)shader;

    if ( rasterizer_debug_options.drawing_mode
      || !rasterizer_debug_options.draw_environment_specular_lights
      || shader_env->environment.specular.brightness <= 0.0
      || specular_light_brightness <= 0.0 )
        return;

    uint16_t flags = shader_env->environment.flags;
    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(
        (flags & (1u << _shader_environment_bump_map_is_specular_mask_bit)) == 0
            ? _dxshader_environment_specular_light_bumped
            : _dxshader_environment_specular_light_flat);
    if ( !effect_shader || !effect_shader->effect )
        return;

    float spot_broadcast = (shader_env->environment.specular.flags & (1u << _shader_environment_specular_overbright_bit)) != 0 ? 4.0f : 2.0f;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment));
    D3DDevice_SetVertexShader(global_d3d_device,
        rasterizer_dx9_shaders_vshader9_get(effect_shader->vshader9 + specular_light_vertex_shader_permutation_index));
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vsh_constants__pointlight, 5,
                                        (uint64_t)3 << 59);

    rasterizer_set_texture_for_effect(0, 0, 3, shader_env->environment.diffuse.bump_map.index,
                                      shader_permutation_index, effect_shader);
    if ( environment_specular_light_active == 1 )
        rasterizer_set_texture_for_effect(1, 2, 1, rasterizer_specular_distance_attenuation_index, 0, effect_shader);
    else
        rasterizer_set_texture_direct_for_effect(1, rasterizer_specular_distance_attenuation_index, 0, effect_shader);
    rasterizer_set_texture_direct_for_effect(2, global_rasterizer_data->vector_normalization.index, 0, effect_shader);
    rasterizer_set_texture_direct_for_effect(3, global_rasterizer_data->vector_normalization.index, 0, effect_shader);

    /* c10..c12 bump texture-transform block; [7]/[11] receive the animated u/v scroll offsets. */
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

    unsigned int *constants = effect_shader->constants;
    if ( constants )
    {
        float vector[4];

        /* c0: specular brightness, broadcast to all four channels. */
        float specular_intensity = (shader_env->environment.specular.brightness * specular_light_brightness);
        vector[0] = specular_intensity;
        vector[1] = specular_intensity;
        vector[2] = specular_intensity;
        vector[3] = specular_intensity;
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, *constants, (const D3DXVECTOR4 *)vector);

        /* c1: view-perpendicular specular colour, 1. */
        vector[0] = shader_env->environment.specular.view_perpendicular_color.red;
        vector[1] = shader_env->environment.specular.view_perpendicular_color.green;
        vector[2] = shader_env->environment.specular.view_perpendicular_color.blue;
        vector[3] = 1.0f;
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[1],
                                         (const D3DXVECTOR4 *)vector);

        /* c2: view-parallel specular colour, 1. */
        vector[0] = shader_env->environment.specular.view_parallel_color.red;
        vector[1] = shader_env->environment.specular.view_parallel_color.green;
        vector[2] = shader_env->environment.specular.view_parallel_color.blue;
        vector[3] = 1.0f;
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[2],
                                         (const D3DXVECTOR4 *)vector);

        /* c3: spot/point exponent broadcast. */
        vector[0] = spot_broadcast;
        vector[1] = spot_broadcast;
        vector[2] = spot_broadcast;
        vector[3] = spot_broadcast;
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[3],
                                         (const D3DXVECTOR4 *)vector);
    }

    unsigned int pass_count[4];
    effect_shader->effect->lpVtbl->Begin(effect_shader->effect, pass_count, 3);
    for ( unsigned int i = 0; i < pass_count[0]; ++i )
    {
        effect_shader->effect->lpVtbl->BeginPass(effect_shader->effect, i);
        rasterizer_draw_dynamic_triangles_static_vertices(dynamic_triangle_buffer_index, first_triangle_index,
                                                          triangle_count, vertex_buffer);
        effect_shader->effect->lpVtbl->EndPass(effect_shader->effect);
    }
    effect_shader->effect->lpVtbl->End(effect_shader->effect);
}
