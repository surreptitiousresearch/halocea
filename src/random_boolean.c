#include <stdint.h>

extern uint32_t *get_global_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);

uint8_t random_boolean(void)
{
    return seed_random(get_global_random_seed_address()) > 0x8000u;
}
