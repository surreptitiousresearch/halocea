/* render_frustum_sphere_diameter_in_pixels @0x8376CE78 — project a world-space sphere's diameter to an
 * approximate on-screen pixel count: transform the sphere center into view space to get its depth, clamp the
 * depth's magnitude to at least 0.1 to avoid divide-by-zero, then scale the diameter (2*radius) by the
 * world-to-screen projection factor divided by that depth.
 *
 * DEVIATION: the decompiler emitted the depth-magnitude step as a raw `fsel f0,f4,f4,f3` with f3 = -f4; that
 * idiom is fabs(view_z). Reconstructed as fabsf() accordingly. */

#include <math.h>
#include "headers/render_frustum.h"
#include "headers/real_point3d.h"

float render_frustum_sphere_diameter_in_pixels(const render_frustum *frustum, const real_point3d *point,
                                               float radius)
{
    float view_z = ((frustum->world_to_view.n[0][2] * point->n[0])
                    + ((frustum->world_to_view.n[2][2] * point->n[2])
                       + (frustum->world_to_view.n[1][2] * point->n[1])))
                 + frustum->world_to_view.n[3][2];

    float depth = fabsf(view_z);
    if ( depth <= 0.1f )
        depth = 0.1f;

    return ((frustum->projection_world_to_screen.n[1] / depth) * radius) * 2.0f;
}
