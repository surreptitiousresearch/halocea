/* lightning_offset_marker_position @0x83809FF0 — jitter a lightning endpoint: pick a (local-random) offset
 * uniform in ±random_position_bounds per axis, rotate it into world space through the marker matrix, and
 * add it to the position in place. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);

void lightning_offset_marker_position(real_point3d *position, const real_matrix4x3 *matrix,
        const real_vector3d *random_position_bounds)
{
    float random_z = real_seed_random(get_global_local_random_seed_address());
    float random_y = real_seed_random(get_global_local_random_seed_address());
    float random_x = real_seed_random(get_global_local_random_seed_address());

    real_vector3d offset;
    offset.n[0] = (random_x * 2.0f - 1.0f) * random_position_bounds->n[0];
    offset.n[1] = (random_y * 2.0f - 1.0f) * random_position_bounds->n[1];
    offset.n[2] = (random_z * 2.0f - 1.0f) * random_position_bounds->n[2];

    matrix4x3_transform_vector(matrix, &offset, &offset);

    position->n[0] += offset.n[0];
    position->n[1] += offset.n[1];
    position->n[2] += offset.n[2];
}
