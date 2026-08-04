/* set_random_seed @ 0x836876D8 — store a new value in the global random seed. */

#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);

void set_random_seed(unsigned int seed)
{
    *get_global_random_seed_address() = seed;
}
