/* projection_from_vector3d @0x836FBEF8 — choose the dominant axis of a 3D vector (the axis with the
 * largest absolute component), used to pick the coordinate plane onto which 3D geometry is projected
 * for 2D point-in-polygon tests. Returns 0 (x dominant), 1 (y dominant) or 2 (z dominant). */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/ppc_intrinsics.h"  /* __fabs — was implicitly declared (C4013) */

/* Return attested int16_t: all sampled callers extsh r3 (or sth it) after the call. */
int16_t projection_from_vector3d(const real_vector3d *n)
{
    float abs_x = (float)__fabs(n->n[0]);
    float abs_y = (float)__fabs(n->n[1]);
    float abs_z = (float)__fabs(n->n[2]);

    if ( abs_z < abs_y || abs_z < abs_x )
        return abs_y >= abs_x ? 1 : 0;
    return 2;
}
