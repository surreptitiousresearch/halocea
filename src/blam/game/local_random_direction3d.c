/* local_random_direction3d @0x836E0C30 — fill direction with a uniformly-distributed random unit vector
 * drawn from the global local-random seed (the non-deterministic seed used for cosmetic/client-side
 * randomness). */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);

real_vector3d *local_random_direction3d(real_vector3d *direction)
{
    return seed_random_direction3d(get_global_local_random_seed_address(), direction);
}
