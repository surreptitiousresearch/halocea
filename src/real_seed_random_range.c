/* real_seed_random_range @ 0x83722F98 — advance an LCG seed and return a float uniformly in
 * [lower_bound, upper_bound). (0.000015259022 == 1/65535.) */

#include <stdint.h>

float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound)
{
    uint32_t next = 1664525u * *seed + 1013904223u;
    *seed = next;
    return (float)(uint16_t)(next >> 16) * 0.000015259022f * (upper_bound - lower_bound) + lower_bound;
}
