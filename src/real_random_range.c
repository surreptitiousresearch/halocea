/* real_random_range @0x836A77D8 — draw a float uniformly in [lower_bound, upper_bound) from the
 * global (deterministic) random seed. */

#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

float real_random_range(float lower_bound, float upper_bound)
{
    return real_seed_random_range(get_global_random_seed_address(), lower_bound, upper_bound);
}
