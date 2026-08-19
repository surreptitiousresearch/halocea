/* point3d_to_point2d @0x8382BA30 — project an array of 3D points onto the XY plane (drop the z component)
 * into a parallel array of 2D points.
 *
 * DEVIATION: the compiler unrolled this into a 4-wide loop plus a scalar remainder, with rolling pointer
 * arithmetic obscuring which source/destination floats correspond. Hand-traced the absolute float indices
 * for every unrolled access and confirmed each is exactly dst[i].n[0]=src[i].n[0], dst[i].n[1]=src[i].n[1] —
 * restored as the plain equivalent per-element copy. */

#include "headers/real_point3d.h"
#include "headers/real_point2d.h"

void point3d_to_point2d(int count, real_point3d *src, real_point2d *dst)
{
    for ( int i = 0; i < count; i++ )
    {
        dst[i].n[0] = src[i].n[0];
        dst[i].n[1] = src[i].n[1];
    }
}
