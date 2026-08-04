#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);

int random(void)
{
    unsigned int *seed = get_global_random_seed_address();
    return seed_random(seed);
}
