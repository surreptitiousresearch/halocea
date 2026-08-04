/* render_frustum_build_point_flags 0x8376C800 — classify a point against the four side planes of a render
 * frustum, returning a bitmask of the planes the point is on the positive (inside) side of: bit 1 = plane 0,
 * bit 2 = plane 1, bit 8 = plane 2, bit 4 = plane 3. Used for cheap point-in-frustum / clip culling. */

#include <stdint.h>
#include "headers/render_frustum.h"
#include "headers/real_point3d.h"

int16_t render_frustum_build_point_flags(const render_frustum *frustum, const real_point3d *point)
{
    __int16 plane1_flag;
    __int16 plane01_flags;
    __int16 plane2_flag;
    __int16 plane012_flags;
    __int16 plane3_flag;

    plane1_flag = 2;
    if ( (((frustum->world_planes[1].normal.n[0] * point->n[0])
                       + ((frustum->world_planes[1].normal.n[1] * point->n[1])
                               + (frustum->world_planes[1].normal.n[2] * point->n[2])))
               - frustum->world_planes[1].distance) <= 0.0 )
        plane1_flag = 0;

    plane01_flags = ((((frustum->world_planes[0].normal.n[0] * point->n[0])
                                   + ((frustum->world_planes[0].normal.n[1] * point->n[1])
                                           + (frustum->world_planes[0].normal.n[2] * point->n[2])))
                           - frustum->world_planes[0].distance) > 0.0)
                    | plane1_flag;

    plane2_flag = 8;
    if ( (((frustum->world_planes[2].normal.n[0] * point->n[0])
                       + ((frustum->world_planes[2].normal.n[1] * point->n[1])
                               + (frustum->world_planes[2].normal.n[2] * point->n[2])))
               - frustum->world_planes[2].distance) <= 0.0 )
        plane2_flag = 0;

    plane012_flags = plane01_flags | plane2_flag;

    plane3_flag = 4;
    if ( (((point->n[0] * frustum->world_planes[3].normal.n[0])
                       + ((frustum->world_planes[3].normal.n[1] * point->n[1])
                               + (frustum->world_planes[3].normal.n[2] * point->n[2])))
               - frustum->world_planes[3].distance) <= 0.0 )
        plane3_flag = 0;

    return plane012_flags | (unsigned __int16)plane3_flag;
}
