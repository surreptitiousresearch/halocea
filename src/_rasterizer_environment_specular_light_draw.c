/* _rasterizer_environment_specular_light_draw 0x837A4F50 — draw the environment specular dynamic-light pass for
 * one surface batch. Runs when the specular-lights debug toggle is on, the shader's specular brightness is
 * positive, and the global specular light brightness is positive. Selects a specular-light effect shader by the
 * shader's colored-lightmap flag, binds the bump map (stage 0), the distance-attenuation / spot gel map
 * (stage 1, direct or spot variant), and the normalization cube map (stages 2/3), uploads the point-light
 * vertex constants and the bump texture-transform constants, sets the per-light specular colour/power/tint
 * constants, then draws the dynamic triangles once per effect pass.
 *
 * DEVIATION 1: the shader is kept as the decompiler's 40-byte shader[N] type-pun indexing into the underlying
 * shader_environment (shader[7]=diffuse bump fields, shader[15]=specular flags, shader[16]=specular brightness,
 * shader[17]=specular colour/power/tint); LODWORD reinterprets the float bump-map index field as an int tag.
 * DEVIATION 2: the two D3DDevice_SetVertexShaderConstantFN count/mask pairs the decompiler garbled are, per the
 * disassembly, reg 0xD count 5 mask (uint64)3<<59 (point-light constants) and reg 0xA count 3 mask
 * (uint64)3<<60 (texture transform). The texture-transform block is one contiguous 12-float buffer the
 * decompiler split into two arrays; evaluate writes the u/v scroll offsets into elements [7] and [11].
 * DEVIATION 3: canonical 6-arg draw signature; the decompiler reuses one 4-float scratch for all four SetVector
 * uploads (each copied by SetVector before the next fill), expressed here as one reused buffer. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/vertex_buffer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_environment_light_globals.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/global_frame_parameters.h"
#include "headers/hexrays_defs.h"
#include "headers/point2d.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/d3dx_effect_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index);
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern void shader_environment_texture_animation_evaluate(const struct shader *shader, float time_value, float *u_offset, float *v_offset);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                unsigned __int64 PendingMask0);
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void rasterizer_draw_dynamic_triangles_static_vertices(int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer);

void _rasterizer_environment_specular_light_draw(const shader *shader, int16_t shader_permutation_index, int dynamic_triangle_buffer_index, int first_triangle_index, int triangle_count, const vertex_buffer *vertex_buffer)
{
    if ( rasterizer_debug_options.drawing_mode
      || !rasterizer_debug_options.draw_environment_specular_lights
      || shader[16].base.radiosity.color.n[2] <= 0.0
      || specular_light_brightness <= 0.0 )
        return;

    unsigned __int16 flags = shader[1].base.radiosity.flags;
    rasterizer_dx9_shader *effect_shader = rasterizer_shader_select(
        (flags & 2) == 0 ? _dxshader_environment_specular_light_bumped
                         : _dxshader_environment_specular_light_flat);
    if ( !effect_shader || !effect_shader->effect )
        return;

    float spot_broadcast = (shader[15].base.type & 1) != 0 ? 4.0f : 2.0f;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_environment));
    D3DDevice_SetVertexShader(global_d3d_device,
        rasterizer_dx9_shaders_vshader9_get(effect_shader->vshader9 + specular_light_vertex_shader_permutation_index));
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vsh_constants__pointlight, 5,
                                        (unsigned __int64)3 << 59);

    rasterizer_set_texture_for_effect(0, 0, 3, *(int *)&shader[7].base.radiosity.tint_color.n[2],
                                      shader_permutation_index, effect_shader);
    if ( environment_specular_light_active == 1 )
        rasterizer_set_texture_for_effect(1, 2, 1, rasterizer_specular_distance_attenuation_index, 0, effect_shader);
    else
        rasterizer_set_texture_direct_for_effect(1, rasterizer_specular_distance_attenuation_index, 0, effect_shader);
    rasterizer_set_texture_direct_for_effect(2, global_rasterizer_data->vector_normalization.index, 0, effect_shader);
    rasterizer_set_texture_direct_for_effect(3, global_rasterizer_data->vector_normalization.index, 0, effect_shader);

    /* c10..c12 bump texture-transform block; [7]/[11] receive the animated u/v scroll offsets. */
    float texture_transform_constants[12];
    texture_transform_constants[0] = *(float *)&shader[7].base.physics;
    texture_transform_constants[1] = *(float *)&shader[7].base.type;
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

    unsigned int *constants = effect_shader->constants;
    if ( constants )
    {
        float vector[4];

        /* c0: specular brightness, broadcast to all four channels. */
        float specular_intensity = (shader[16].base.radiosity.color.n[2] * specular_light_brightness);
        vector[0] = specular_intensity;
        vector[1] = specular_intensity;
        vector[2] = specular_intensity;
        vector[3] = specular_intensity;
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, *constants, (const D3DXVECTOR4 *)vector);

        /* c1: specular flags, power, colour[0], 1. */
        vector[0] = *(float *)&shader[17].base.radiosity.flags;
        vector[1] = shader[17].base.radiosity.power;
        vector[2] = shader[17].base.radiosity.color.n[0];
        vector[3] = 1.0f;
        effect_shader->effect->lpVtbl->SetVector(effect_shader->effect, effect_shader->constants[1],
                                         (const D3DXVECTOR4 *)vector);

        /* c2: specular colour[1], colour[2], tint[0], 1. */
        vector[0] = shader[17].base.radiosity.color.n[1];
        vector[1] = shader[17].base.radiosity.color.n[2];
        vector[2] = shader[17].base.radiosity.tint_color.n[0];
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
