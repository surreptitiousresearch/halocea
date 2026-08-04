/* bsp3d_get_plane_from_designator @0x8374012C — resolve a signed BSP plane designator to a plane: a
 * non-negative designator copies the plane verbatim; a negative one returns the same plane negated
 * (back-facing). Planes are 16 bytes, indexed by the designator's magnitude. */

#include "headers/bsp3d.h"
#include "headers/real_plane3d.h"

real_plane3d *bsp3d_get_plane_from_designator(const bsp3d *bsp, int plane_designator, real_plane3d *result)
{
    real_plane3d *plane = &((real_plane3d *)bsp->planes.address)[plane_designator];
    if ( plane_designator >= 0 )
    {
        *result = *plane;
    }
    else
    {
        result->n.n[0] = -plane->n.n[0];
        result->n.n[1] = -plane->n.n[1];
        result->n.n[2] = -plane->n.n[2];
        result->d = -plane->d;
    }
    return result;
}
