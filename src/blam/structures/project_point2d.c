/* project_point2d @0x8373FFE0 — lift a 2D point back to 3D by placing it on a plane. The (projection, sign) pair
 * selects which axis is the "depth" axis and the mapping of the 2D coordinates onto the other two axes; the
 * depth component is then solved from the plane equation (n . p == d). When the plane is parallel to
 * the projection axis (near-zero normal component) the depth is set to 0. Returns the output point. */

#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/fused_math.h"

extern double __fabs(double x);

real_point3d *project_point2d(const real_point2d *p2d, const real_plane3d *plane, int16_t projection,
                              uint8_t sign, real_point3d *p3d)
{
    int axis_u = global_projection3d_mappings[projection][sign][0];
    int axis_v = global_projection3d_mappings[projection][sign][1];

    p3d->n[axis_u] = p2d->n[0];
    p3d->n[axis_v] = p2d->n[1];

    if (__fabs(plane->n.n[projection]) >= 0.0001f)
    {
        /* DEVIATION: fused chain — fnmsubs @0x83740068 (d - n[axis_u]*p2d[0]) feeds
         * fnmsubs @0x83740074 (- n[axis_v]*p2d[1]) then fdivs @0x83740078; the axis_u term
         * is subtracted FIRST and each step single-rounds — a plain expression double-rounds. */
        float partial = fused_nmsub(plane->n.n[axis_u], p2d->n[0], plane->d);
        float numerator = fused_nmsub(plane->n.n[axis_v], p2d->n[1], partial);
        p3d->n[projection] = numerator / plane->n.n[projection];
    }
    else
        p3d->n[projection] = 0.0f;
    return p3d;
}
