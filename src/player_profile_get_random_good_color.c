/* player_profile_get_random_good_color @0x83765018 — random color index from the "good" subset [0,3]. */

#include <stdint.h>

extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int player_profile_get_random_good_color(void)
{
    return seed_random_range(get_global_local_random_seed_address(), 0, 3);
}
