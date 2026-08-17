/* point_in_cone3d @0x836FE310 */
#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

/* True if `point` lies inside the 3D cone with apex `base`, axis `direction`
 * (unit), length `distance` and half-angle whose cosine is `cosine`.
 * DEVIATION: `distance`/`cosine` are floats passed in FP regs; decompiler
 * widened them to double. */
uint8_t point_in_cone3d(const real_point3d *point, const real_point3d *base, const real_vector3d *direction, float distance, float cosine)
{
    float dx = point->n[0] - base->n[0];
    float dy = point->n[1] - base->n[1];
    float dz = point->n[2] - base->n[2];
    float axial = direction->n[0] * dx + direction->n[1] * dy + direction->n[2] * dz;

    if ( axial < 0.0f )
        return 0;
    if ( axial > distance )
        return 0;
    if ( axial * axial < (dx * dx + dy * dy + dz * dz) * cosine * cosine )
        return 0;
    return 1;
}
