/* real_seed_random_range @ 0x83722F98 — advance an LCG seed and return a float uniformly in
 * [lower_bound, upper_bound). (0.000015259022 == 1/65535.) */

#include <stdint.h>
#include "headers/fused_math.h"

float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound)
{
    uint32_t next = 1664525u * *seed + 1013904223u;
    *seed = next;
    /* DEVIATION: fsubs @0x83722FA4, fcfid @0x83722FCC + frsp @0x83722FD0 + fmuls @0x83722FD4,
     * then fmadds @0x83722FD8 — the final scale-into-range is FUSED (unit*delta + lower with one
     * rounding); every intermediate stays single-precision float. */
    float delta = upper_bound - lower_bound;
    float unit = (float)(uint16_t)(next >> 16) * 0.000015259022f;
    return fused_madd(unit, delta, lower_bound);
}
