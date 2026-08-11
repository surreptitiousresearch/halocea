/* effect_real_random_range @0x836E1278 — random value in [lower, upper], with the lower bound and the
 * span each optionally scaled by the effect's scale_a / scale_b driver values. Whether each scaling
 * applies is controlled by bit `first_bit_index` (for the lower bound / scale_a-on-lower) and bit
 * `first_bit_index + 1` (for the span) of the scale_a_flags and scale_b_flags masks. */

#include <stdint.h>
#include "headers/effect_datum.h"

extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

/* DEVIATION: prior reconstruction carried two phantom leading flag params (unused_a/unused_b) that
 * shifted the mapping; disasm shows only 7 args — r7=scale_a_flags, r8=scale_b_flags, r9(extsh)=
 * first_bit_index (f1/f2 shadow r5/r6). Removed the phantom params. */
extern float effect_scale(const effect_datum *effect, float value, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t bit_index);
float effect_real_random_range(uint32_t *seed, const effect_datum *effect, float lower_bound,
        float upper_bound, unsigned int scale_a_flags, unsigned int scale_b_flags,
        int16_t first_bit_index)
{
    /* DEVIATION: collapsed 2 inlined effect_scale@0x836E1210 copies (base @first_bit_index, span @first_bit_index+1); donor is zero-xref globally, inlined even within this sibling's own compiled body */
    float base = effect_scale(effect, lower_bound, scale_a_flags, scale_b_flags, first_bit_index);
    float span = effect_scale(effect, upper_bound - lower_bound, scale_a_flags, scale_b_flags, first_bit_index + 1);

    return real_seed_random_range(seed, 0.0f, span) + base;
}
