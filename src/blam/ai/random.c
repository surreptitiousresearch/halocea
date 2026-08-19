#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);

/* random @0x836D8488 — draw the next 16-bit value from the global random seed. The return is
 * inherited verbatim from seed_random (no widening in the epilogue), so it is uint16_t. */

uint16_t random(void)
{
    uint32_t *seed = get_global_random_seed_address();
    return seed_random(seed);
}
