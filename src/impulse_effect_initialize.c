/* impulse_effect_initialize @0x836E15E0 — fill in an effect_datum's impulse parameters: the two scale
 * values, a base color (defaulting to white), and an optional directional impulse field.
 *
 * Deviation from decompiler: it invented two unused parameters for the r4/r5 phantom GPR slots that the
 * float args (scale_a/scale_b) reserve on PPC. The disassembly shows only five real arguments: r3=effect,
 * f1=scale_a, f2=scale_b, r6=color, r7=impulse_field. */

#include "headers/effect_datum.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/blam_data_globals.h"

void impulse_effect_initialize(effect_datum *effect, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field)
{
    effect->scale_a = scale_a;
    effect->scale_b = scale_b;

    const real_rgb_color *base_color = color ? color : global_real_rgb_white;
    effect->color = *base_color;

    if ( impulse_field )
    {
        effect->impulse_field = *impulse_field;
    }
    else
    {
        /* identifier (0x00) left untouched, matching the disassembly */
        effect->impulse_field.translational = nullptr;
        effect->impulse_field.angular = nullptr;
    }
}
