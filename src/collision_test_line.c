/* collision_test_line @0x836B10E0 — convenience wrapper around collision_test_vector that casts a ray between
 * two explicit endpoints (delta = point1 - point0). Returns whatever collision_test_vector reports.
 *
 * Deviation: Hex-Rays widens the single-precision subtractions to double (fp7/fp6) — kept as float. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"

extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

uint8_t collision_test_line(unsigned int flags, const real_point3d *point0, const real_point3d *point1,
                            int ignore_object_index, collision_result *collision)
{
    real_vector3d delta;
    delta.n[0] = point1->n[0] - point0->n[0];
    delta.n[1] = point1->n[1] - point0->n[1];
    delta.n[2] = point1->n[2] - point0->n[2];
    return collision_test_vector(flags, point0, &delta, ignore_object_index, collision);
}
