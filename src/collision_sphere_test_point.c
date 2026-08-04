/* collision_sphere_test_point @0x83804E98 — test whether a point lies inside a sphere feature. On a hit writes the
 * outward surface normal/plane and the penetration depth (radius − distance). Returns TRUE if inside. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_sphere.h"

extern float __fsqrts(float);

uint8_t collision_sphere_test_point(const collision_sphere *sphere, const real_point3d *point,
                                float *depth, real_plane3d *plane)
{
    float dx = point->n[0] - sphere->center.n[0];
    float dz = point->n[2] - sphere->center.n[2];
    float dy = point->n[1] - sphere->center.n[1];
    float distance_squared = ((dy * dy) + ((dz * dz) + (dx * dx)));
    if ( distance_squared >= (sphere->radius * sphere->radius) )
        return 0;

    float distance = __fsqrts(distance_squared);
    if ( distance <= 0.0 )
    {
        plane->n.n[0] = 0.0;
        plane->n.n[1] = 0.0;
        plane->n.n[2] = 1.0;
    }
    else
    {
        float inverse = ((float)1.0 / distance);
        plane->n.n[0] = (point->n[0] - sphere->center.n[0]) * inverse;
        plane->n.n[1] = inverse * dy;
        plane->n.n[2] = dz * inverse;
    }
    plane->d = ((plane->n.n[0] * sphere->center.n[0])
                            + ((sphere->center.n[1] * plane->n.n[1])
                                    + (sphere->center.n[2] * plane->n.n[2])))
                    + sphere->radius;
    *depth = sphere->radius - distance;
    return 1;
}
