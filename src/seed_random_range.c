/* seed_random_range @ 0x83723008 — advance an LCG seed and return an integer uniformly in
 * [lower_bound, upper_bound). Return attested int16_t: extsh r3 before blr + callers extsh after. */

#include <stdint.h>

int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound)
{
    int span = upper_bound - lower_bound;
    uint32_t next = 1664525u * *seed + 1013904223u;
    *seed = next;
    return (int16_t)(((span * (next >> 16)) >> 16) + lower_bound); /* cast models extsh */
}
