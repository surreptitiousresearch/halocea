/* random_direction3d @0x836A7820 — pick a uniformly random unit direction using
 * the global random seed. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);

real_vector3d *random_direction3d(real_vector3d *direction)
{
    return seed_random_direction3d(get_global_random_seed_address(), direction);
}
