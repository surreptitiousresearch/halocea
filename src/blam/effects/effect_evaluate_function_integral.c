/* effect_evaluate_function_integral @0x836E1FD8 — the cumulative fraction of an effect's particle output
 * emitted by the given event fraction, for each distribution-function shape. A fraction of -1 (no previous
 * frame) yields 0. Functions (DB enum): start = all at once, end = step at the end, constant = linear,
 * buildup = quadratic (late), falloff = inverse-quadratic (early), quadratic = smoothstep.
 *
 * NOTE: an out-of-range function index returns an indeterminate value in the original (uninitialized
 * stack); reconstructed as the input fraction. */

#include <stdint.h>
#include "headers/effect_particle_distribution_function.h"

float effect_evaluate_function_integral(int16_t function, float fraction)
{
    if ( fraction == -1.0 )
        return 0.0f;

    switch ( function )
    {
        case _effect_particle_distribution_function_start:
            return 1.0f;
        case _effect_particle_distribution_function_end:
            return fraction >= 1.0 ? 1.0f : 0.0f;
        case _effect_particle_distribution_function_constant:
            return fraction;
        case _effect_particle_distribution_function_buildup:
            return (fraction * fraction);
        case _effect_particle_distribution_function_falloff:
            return (((float)2.0 - fraction) * fraction);
        case _effect_particle_distribution_function_quadratic:
            return ((-(((fraction * 2.0f) - 3.0f) * fraction)) * fraction);
        default:
            return fraction;   /* out of range: original returns indeterminate */
    }
}
