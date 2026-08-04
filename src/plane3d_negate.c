/* plane3d_negate @0x836C6A20 — negate a plane (flip its normal and distance) into result. */

#include "headers/real_plane3d.h"

real_plane3d * plane3d_negate(const real_plane3d *plane, real_plane3d *result)
{
    result->n.n[0] = -plane->n.n[0];
    result->n.n[1] = -plane->n.n[1];
    result->n.n[2] = -plane->n.n[2];
    result->d = -plane->d;
    return result;
}
