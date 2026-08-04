#include <stdint.h>

float real_seed_random(uint32_t *seed)
{
    uint32_t next = 1664525u * *seed + 1013904223u;
    *seed = next;
    return (float)(uint16_t)(next >> 16) * 0.000015259022f;
}
