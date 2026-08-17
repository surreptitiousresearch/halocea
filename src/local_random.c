/* local_random @0x8373D928 */
#include <stdint.h>

extern uint32_t *get_global_local_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);

uint16_t local_random(void)
{
    return seed_random(get_global_local_random_seed_address());
}
