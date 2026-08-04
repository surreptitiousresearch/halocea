/* local_random_range @0x836A7858 — return a random integer in [lower_bound, upper_bound] drawn from the
 * global local-random seed (non-deterministic, client-side randomness). */

#include <stdint.h>

extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int local_random_range(__int16 lower_bound, __int16 upper_bound)
{
    return seed_random_range(get_global_local_random_seed_address(), lower_bound, upper_bound);
}
