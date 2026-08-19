/* bsp3d_get_plane_from_designator @0x83740120 — resolve a signed BSP plane designator to a plane: a
 * non-negative designator copies the plane verbatim; a negative one returns the same plane negated
 * (back-facing). Planes are 16 bytes, indexed by the designator's magnitude.
 *
 * DEVIATION: bit 31 of the designator is the orientation flag, not index data. The binary never masks
 * before indexing because the 16-byte stride shift discards that bit in 32-bit arithmetic
 * (`slwi r10, r4, 4` @0x83740124), and isolates it separately for the facing test
 * (`clrrwi r9, r4, 31` @0x83740128). The `& 0x7FFFFFFF` below is therefore a proven no-op on the
 * as-built target and keeps the index in range where ptrdiff_t is wider (x64); the facing test still
 * reads the unmasked value. */

#include "headers/bsp3d.h"
#include "headers/real_plane3d.h"

real_plane3d *bsp3d_get_plane_from_designator(const bsp3d *bsp, int plane_designator, real_plane3d *result)
{
    real_plane3d *plane = &((real_plane3d *)bsp->planes.address)[plane_designator & 0x7FFFFFFF];
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
