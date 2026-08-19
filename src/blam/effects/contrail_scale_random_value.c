/* contrail_scale_random_value @0x8373EF30 — pick a random value in [lower, upper], optionally density-
 * scaling the lower bound (when scale_flags bit `first_bit_index` is set) and/or the span (when bit
 * `first_bit_index + 1` is set). Used for contrail/particle emission parameters that can scale with the
 * emitter's density. */

#include <stdint.h>
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

float contrail_scale_random_value(float density, float lower_bound, float upper_bound,
                                  unsigned int scale_flags, int16_t first_bit_index)
{
    float base = lower_bound;
    float span;

    if ( ((1 << first_bit_index) & scale_flags) != 0 )
        base = density * lower_bound;
    span = upper_bound - lower_bound;
    if ( ((1 << (first_bit_index + 1)) & scale_flags) != 0 )
        span = (upper_bound - lower_bound) * density;

    return real_seed_random_range(get_global_local_random_seed_address(), 0.0f, span) + base;
}
