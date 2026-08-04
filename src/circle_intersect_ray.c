/* circle_intersect_ray @0x8382F888 — ray/circle intersection test: does the ray from `point` along
 * `direction` hit the circle of `radius` centered at `center`? On a hit, writes the distance along the ray
 * to the first intersection (0 if the ray origin is already inside/on the circle) into `*distance`.
 *
 * DEVIATION: the decompiler fabricated a 6th `float *a6` parameter and its own body reads the true `point`
 * and `direction` args out of the wrong registers relative to its own declared 6-arg signature; the DB's own
 * prototype (5 args) and disasm_range(0x8382F888,0x8382F914) agree this is the same float-arg-reserves-a-GPR-
 * slot ABI quirk documented for collision_bsp_test_pill_new.c: `radius` (float, in f1) still reserves r4,
 * shifting `point`/`direction`/`distance` into r5/r6/r7. The result is written back through `distance`
 * itself (`stfs f13, 0(r7)`), not a separate output pointer. */

#include <stdint.h>
#include <math.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

uint8_t circle_intersect_ray(const real_point2d *center, float radius, const real_point2d *point, const real_vector2d *direction, float *distance)
{
    float dx = center->n[0] - point->n[0];
    float dy = center->n[1] - point->n[1];
    float projection = direction->n[0] * dx + direction->n[1] * dy;

    if ( projection <= 0.0f )
        return 0;

    float distance_squared = dx * dx + dy * dy;
    float discriminant_offset = distance_squared - radius * radius;

    if ( discriminant_offset <= 0.0f )
    {
        *distance = 0.0f;
        return 1;
    }

    float discriminant = projection * projection - discriminant_offset;

    if ( discriminant < 0.0f )
        return 0;

    *distance = projection - sqrtf(discriminant);
    return 1;
}
