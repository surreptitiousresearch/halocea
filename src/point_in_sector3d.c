/* point_in_sector3d @0x836FE400 */
#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

/* point_in_sector3d — is `point` inside the 3D cone (sector) rooted at `base`, opening along
 * `direction` (unit), with slant range `distance` and half-angle whose cosine is `cosine`?
 * Rejects on range, on being behind the axis, then on the angle test comparing the squared axial
 * projection against distance^2 * cosine^2. */
uint8_t point_in_sector3d(const real_point3d *point, const real_point3d *base, const real_vector3d *direction, float distance, float cosine)
{
    float dz = point->n[2] - base->n[2];
    float dy = point->n[1] - base->n[1];
    float dx = point->n[0] - base->n[0];
    float distance_squared = (dy * dy) + ((dx * dx) + (dz * dz));
    float projection;

    if (distance_squared > (distance * distance))
        return 0;

    projection = (direction->n[0] * dx) + ((direction->n[2] * dz) + (direction->n[1] * dy));
    if (projection < 0.0f)
        return 0;

    if ((projection * projection) < ((distance_squared * cosine) * cosine))
        return 0;

    return 1;
}
