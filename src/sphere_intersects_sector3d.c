/* sphere_intersects_sector3d @0x836FF890 — test a sphere against a 3D sector (a cone of half-angle
 * (sine, cosine) truncated at `distance` along `direction` from `base`). First rejects by the sphere's
 * projection onto the sector axis, then expands the sector by the sphere radius: the apex is pulled back
 * radius/sine along the axis, the length grows by radius/sine + radius, and the sphere center is tested as
 * a point against the expanded sector.
 *
 * DEVIATION (decompiler bug class 1, FPR-shadow): the decompiler emitted 8 parameters, mis-labeling r5 as
 * `direction` and inventing a trailing `a8` for r6. Disasm (r4 is a burned shadow slot for f1/radius)
 * confirms the DB's 7-arg prototype: r3=center, f1=radius, r5=base, r6=direction, f2=distance, f3=sine,
 * f4=cosine. Renamed accordingly. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern uint8_t point_in_sector3d(const real_point3d *point, const real_point3d *base, const real_vector3d *direction, float distance, float cosine);

unsigned __int8 sphere_intersects_sector3d(const real_point3d *center, float radius, const real_point3d *base,
        const real_vector3d *direction, float distance, float sine, float cosine)
{
    float projection = direction->n[2] * (center->n[2] - base->n[2])
                     + direction->n[0] * (center->n[0] - base->n[0])
                     + direction->n[1] * (center->n[1] - base->n[1]);

    if ( projection < -radius )
        return 0;
    if ( projection > radius + distance )
        return 0;

    float apex_pullback = radius / sine;
    real_point3d expanded_base;
    expanded_base.n[0] = direction->n[0] * -apex_pullback + base->n[0];
    expanded_base.n[1] = direction->n[1] * -apex_pullback + base->n[1];
    expanded_base.n[2] = direction->n[2] * -apex_pullback + base->n[2];

    return point_in_sector3d(center, &expanded_base, direction,
                             apex_pullback + radius + distance, cosine) != 0;
}
