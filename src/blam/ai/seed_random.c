/* seed_random @0x83722FE0 — advance a linear-congruential RNG state in place and return the high 16 bits of
 * the new state (Numerical Recipes LCG constants).
 * attested: callers re-normalize r3 with clrlwi ..,16 (weapon_trigger_fire, decal_insert,
 * random_boolean, actor_start_burst) — declared return is 16-bit. */

#include <stdint.h>

uint16_t seed_random(uint32_t *seed)
{
    uint32_t next = 1664525u * *seed + 1013904223u;
    *seed = next;
    return (uint16_t)(next >> 16);
}
