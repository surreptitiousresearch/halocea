/* project_point3d @0x836FBF78 — project a 3D point onto a 2D plane by selecting
 * two of its three axes according to the projection/sign lookup table. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/projection3d_globals.h"

real_point2d * project_point3d(const real_point3d *p3d, int16_t projection, uint8_t sign, real_point2d *p2d)
{
    const int16_t *axes = global_projection3d_mappings[projection][sign];

    p2d->n[0] = p3d->n[axes[0]];
    p2d->n[1] = p3d->n[axes[1]];
    return p2d;
}
