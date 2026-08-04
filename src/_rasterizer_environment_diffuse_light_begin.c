/* _rasterizer_environment_diffuse_light_begin 0x83789318 — set up the diffuse lighting pass for one dynamic
 * light. Selects the diffuse-light pixel-shader effect, binds the light's projected gel map, animates the gel
 * frame by the per-axis yaw/pitch/roll periodic functions (scaled to radians), and uploads the rotated light
 * basis + position + inverse-radius into the vertex-shader constant block (registers 0xD..0x11). Finally it
 * pushes the light color into the effect's first parameter. No-op unless debug drawing is in normal mode and
 * environment diffuse lights are enabled.
 *
 * DEVIATION: the decompiler reported the D3DDevice_SetVertexShaderConstantFN count/mask as (0x18000000, v13);
 * the disassembly shows count = 5 and PendingMask0 = (uint64)3 << 59, used here. */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_lights.h"
#include "headers/point_light_definition.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/rasterizer_shader_helpers.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/global_frame_parameters.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/point2d.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

#include "headers/point2d.h"
extern point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index, rasterizer_dx9_shader *dxeffect_shader);
extern float periodic_function_evaluate(int16_t function_type, float time);
extern void matrix4x3_rotation_from_angles(real_matrix4x3 *matrix, float yaw, float pitch, float roll);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern float normalize3d(real_vector3d *v);

void _rasterizer_environment_diffuse_light_begin(int light_index)
{
    rasterizer_light_submit_parameters *light;
    rasterizer_dx9_shader *shader;
    float yaw, pitch, roll;
    real_vector3d gel_forward;
    real_vector3d gel_up;
    real_vector3d gel_side;
    real_matrix4x3 gel_rotation;
    float vsh_constants[20];
    float light_color[4];

    if ( rasterizer_debug_options.drawing_mode || !rasterizer_debug_options.draw_environment_diffuse_lights )
        return;

    light = &rasterizer_lights.lights[light_index];
    shader = rasterizer_shader_select(_dxshader_environment_diffuse_lights);
    if ( shader && shader->effect )
        rasterizer_set_texture_for_effect(1, 2, 1, light->definition->gel.map.index, 0, shader);

    yaw = (periodic_function_evaluate(light->definition->gel.yaw_function,
                      global_frame_parameters.game_time_sec / light->definition->gel.yaw_period) * TWO_PI);
    pitch = (periodic_function_evaluate(light->definition->gel.pitch_function,
                      global_frame_parameters.game_time_sec / light->definition->gel.pitch_period) * TWO_PI);
    roll = (periodic_function_evaluate(light->definition->gel.roll_function,
                      global_frame_parameters.game_time_sec / light->definition->gel.roll_period) * TWO_PI);

    matrix4x3_rotation_from_angles(&gel_rotation, yaw, pitch, roll);
    matrix4x3_transform_normal(&gel_rotation, &light->forward, &gel_forward);
    matrix4x3_transform_normal(&gel_rotation, &light->up, &gel_up);

    /* gel_side = gel_up x gel_forward */
    gel_side.n[0] = (gel_up.n[2] * gel_forward.n[1]) - (gel_up.n[1] * gel_forward.n[2]);
    gel_side.n[1] = (gel_up.n[0] * gel_forward.n[2]) - (gel_up.n[2] * gel_forward.n[0]);
    gel_side.n[2] = (gel_up.n[1] * gel_forward.n[0]) - (gel_up.n[0] * gel_forward.n[1]);
    normalize3d(&gel_side);

    vsh_constants[0] = light->position.n[0];
    vsh_constants[3] = ((float)0.5 / light->radius);
    vsh_constants[1] = light->position.n[1];
    vsh_constants[7] = 1.0f;
    vsh_constants[2] = light->position.n[2];
    vsh_constants[11] = 1.0f;
    vsh_constants[15] = 1.0f;
    vsh_constants[16] = 0.0f;
    vsh_constants[17] = 0.0f;
    vsh_constants[18] = 0.0f;
    vsh_constants[19] = 1.0f;
    vsh_constants[4] = -gel_forward.n[0];
    vsh_constants[5] = -gel_forward.n[1];
    vsh_constants[6] = -gel_forward.n[2];
    vsh_constants[8] = -gel_side.n[0];
    vsh_constants[9] = -gel_side.n[1];
    vsh_constants[10] = -gel_side.n[2];
    vsh_constants[12] = -gel_up.n[0];
    vsh_constants[13] = -gel_up.n[1];
    vsh_constants[14] = -gel_up.n[2];
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, vsh_constants, 5, (unsigned __int64)3 << 59);

    light_color[0] = light->color.n[0];
    light_color[3] = 1.0f;
    light_color[1] = light->color.n[1];
    light_color[2] = light->color.n[2];
    shader->effect->lpVtbl->SetVector(shader->effect, *shader->constants, (const D3DXVECTOR4 *)light_color);
}
