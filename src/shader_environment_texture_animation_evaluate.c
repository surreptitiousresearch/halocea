/* shader_environment_texture_animation_evaluate @0x83755BE8 — compute the animated u and v base-map texture
 * offsets for an environment shader at a given time. Each axis runs a periodic function (over time / period)
 * scaled by a per-axis amplitude.
 *
 * DEVIATION: the decompiler rendered the shader-tag field accesses through a scaled `shader[8].base...`
 * expression; the real offsets are taken from the disassembly. They land in the environment shader's diffuse
 * block, now accessed via the DB-modeled shader_environment struct:
 *   diffuse.u_animation_function/period/scale (tag +0x150/+0x154/+0x158)
 *   diffuse.v_animation_function/period/scale (tag +0x15C/+0x160/+0x164) */

#include <stdint.h>
#include "headers/shader_environment.h"

extern float periodic_function_evaluate(int16_t function_type, float time);

void shader_environment_texture_animation_evaluate(const struct shader *shader, float time_value,
                                                   float *u_offset, float *v_offset)
{
    const shader_environment *environment_shader = (const shader_environment *)shader;
    const shader_environment_diffuse_properties *diffuse = &environment_shader->environment.diffuse;

    *u_offset = periodic_function_evaluate(diffuse->u_animation_function,
                    time_value / diffuse->u_animation_period)
                * diffuse->u_animation_scale;

    *v_offset = periodic_function_evaluate(diffuse->v_animation_function,
                    time_value / diffuse->v_animation_period)
                * diffuse->v_animation_scale;
}
