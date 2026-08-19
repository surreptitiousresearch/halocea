/* point_in_sector2d @0x836FE390 */
#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

/* True if `point` lies inside the 2D circular sector centered at `base`, axis
 * `direction` (unit), radius `distance` and half-angle whose cosine is
 * `cosine`.  Differs from point_in_cone2d in gating on radial distance
 * (dx^2+dy^2 <= distance^2) rather than axial projection.
 * DEVIATION: `distance`/`cosine` are floats passed in FP regs; decompiler
 * widened them to double. */
uint8_t point_in_sector2d(const real_point2d *point, const real_point2d *base, const real_vector2d *direction, float distance, float cosine)
{
    float dx = point->n[0] - base->n[0];
    float dy = point->n[1] - base->n[1];
    float radius_squared = dx * dx + dy * dy;

    if ( radius_squared > distance * distance )
        return 0;

    float axial = direction->n[0] * dx + direction->n[1] * dy;
    if ( axial < 0.0f )
        return 0;
    if ( axial * axial < radius_squared * cosine * cosine )
        return 0;
    return 1;
}
