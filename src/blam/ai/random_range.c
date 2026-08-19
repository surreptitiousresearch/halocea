/* random_range @0x8368C8B0 — integer uniformly in [lower_bound, upper_bound)
 * using the global random seed. */

#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int16_t random_range(int16_t lower_bound, int16_t upper_bound)
{
    return seed_random_range(get_global_random_seed_address(), lower_bound, upper_bound);
}
