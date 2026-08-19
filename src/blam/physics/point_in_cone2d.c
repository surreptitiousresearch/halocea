/* point_in_cone2d @0x836FE2A8 */
#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

/* True if `point` lies inside the 2D cone with apex `base`, axis `direction`
 * (unit), length `distance` and half-angle whose cosine is `cosine`.
 * DEVIATION: `distance`/`cosine` are floats passed in FP regs; decompiler
 * widened them to double. */
uint8_t point_in_cone2d(const real_point2d *point, const real_point2d *base, const real_vector2d *direction, float distance, float cosine)
{
    float dx = point->n[0] - base->n[0];
    float dy = point->n[1] - base->n[1];
    float axial = direction->n[0] * dx + direction->n[1] * dy;

    if ( axial < 0.0f )
        return 0;
    if ( axial > distance )
        return 0;
    if ( axial * axial < (dx * dx + dy * dy) * cosine * cosine )
        return 0;
    return 1;
}
