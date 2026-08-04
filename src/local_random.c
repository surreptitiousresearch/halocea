#include <stdint.h>

extern uint32_t *get_global_local_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);

int local_random(void)
{
    unsigned int *seed = get_global_local_random_seed_address();
    return seed_random(seed);
}
