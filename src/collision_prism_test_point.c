/* collision_prism_test_point @0x83805108 — test whether a point lies inside a prism feature: between the face
 * plane and the plane offset by `height`, and inside the convex polygon when projected onto the prism's dominant
 * axis. On a hit writes the top-face plane and the penetration depth (height − distance above the face).
 * Returns TRUE if inside. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/collision_prism.h"

extern const __int16 global_projection3d_mappings[1][6][2];

uint8_t collision_prism_test_point(const collision_prism *prism, const real_point3d *point,
                                   float *depth, real_plane3d *plane)
{
    float above = (((prism->plane.n.n[2] * point->n[2])
                                + ((prism->plane.n.n[0] * point->n[0])
                                        + (prism->plane.n.n[1] * point->n[1])))
                        - prism->plane.d);
    if ( above < 0.0 || above >= prism->height )
        return 0;

    int point_count = prism->point_count;
    int map = 2 * prism->projection_axis + prism->projection_sign;
    __int16 axis_a = global_projection3d_mappings[0][map][0];
    __int16 axis_b = global_projection3d_mappings[0][map][1];

    float projected[3];
    projected[0] = (prism->plane.n.n[0] * -above) + point->n[0];
    projected[1] = (prism->plane.n.n[1] * -above) + point->n[1];
    projected[2] = (prism->plane.n.n[2] * -above) + point->n[2];
    float a = projected[axis_a];
    float b = projected[axis_b];

    if ( point_count > 0 )
    {
        for ( int i = 1; ; ++i )
        {
            const real_point2d *current = &prism->points[i - 1];
            int next = (i < point_count) ? i : 0;
            float cross = (((prism->points[next].n[1] - b) * (current->n[0] - a))
                                - ((current->n[1] - b) * (prism->points[next].n[0] - a)));
            if ( cross < 0.0 )
                return 0;            /* outside this edge */
            if ( i >= point_count )
                break;               /* passed every edge → inside */
        }
    }

    plane->n = prism->plane.n;
    plane->d = prism->plane.d + prism->height;
    *depth = prism->height - above;
    return 1;
}
