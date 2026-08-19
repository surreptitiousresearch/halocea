/* local_random_vector_in_cone3d @0x8373EEA0 */
#include <stdint.h>
#include "headers/real_vector3d.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);

real_vector3d *local_random_vector_in_cone3d(const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result)
{
    unsigned int *seed = get_global_local_random_seed_address();

    return seed_random_vector_in_cone3d(seed, axis, inner_cone_angle, outer_cone_angle, result);
}
