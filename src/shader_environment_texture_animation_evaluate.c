/* shader_environment_texture_animation_evaluate @0x83755BE8 — compute the animated u and v base-map texture
 * offsets for an environment shader at a given time. Each axis runs a periodic function (over time / period)
 * scaled by a per-axis amplitude.
 *
 * DEVIATION: the decompiler knew only the 40-byte `shader` base, so it folded the byte offsets into
 * subscripts on it (`shader[8].base.radiosity.color.n[2]` = 8 * 40 + 0x10 = 0x150, and so on). All six
 * reads are members of shader_environment's own body, which begins at +0x28; disasm-confirmed:
 *   `lhz 0x150` / `lfs 0x154` / `lfs 0x158` @0x83755C04/0x83755BF8/0x83755C18
 *        -> environment.diffuse.u_animation_function / _period / _scale
 *   `lhz 0x15C` / `lfs 0x160` / `lfs 0x164` @0x83755C24/0x83755C28/0x83755C34
 *        -> environment.diffuse.v_animation_function / _period / _scale */

#include <stdint.h>
#include "headers/shader_environment.h"

extern float periodic_function_evaluate(int16_t function_type, float time);
/* DEVIATION: time_value is a double, not a float — `fdiv f1, f1, f0` @0x83755C0C with no entry frsp; callers load it with `lfd f1` (@0x837A5158). */
void shader_environment_texture_animation_evaluate(const struct shader *shader, double time_value,
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
