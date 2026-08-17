/* real_seed_random @0x83722F58 */
#include <stdint.h>

float real_seed_random(uint32_t *seed)
{
    uint32_t next = 1664525u * *seed + 1013904223u;
    *seed = next;
    /* DEVIATION: fcfid @0x83722F88 + frsp @0x83722F8C + fmuls @0x83722F90 — the high half
     * converts to float (exact: <= 0xFFFF) and the 1/65535 scale (lfs 0x37800080) multiplies
     * in SINGLE precision; every intermediate stays a float, no double promotion. */
    float unit = (float)(uint16_t)(next >> 16);
    return unit * 0.000015259022f;
}
