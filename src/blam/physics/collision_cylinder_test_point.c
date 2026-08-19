/* collision_cylinder_test_point @0x83804F78 — test whether a point lies inside a cylinder feature (a capsule body
 * between base and base+height, radius `width`). On a hit writes the radial surface normal/plane and the
 * penetration depth (width − radial distance). Returns TRUE if inside. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_cylinder.h"

extern float normalize3d(real_vector3d *v);

uint8_t collision_cylinder_test_point(const collision_cylinder *cylinder, const real_point3d *point,
                                  float *depth, real_plane3d *plane)
{
    float dx = point->n[0] - cylinder->base.n[0];
    float dy = point->n[1] - cylinder->base.n[1];
    float dz = point->n[2] - cylinder->base.n[2];

    float projection = ((cylinder->height.n[2] * dz)
                             + ((cylinder->height.n[0] * dx) + (cylinder->height.n[1] * dy)));
    if ( projection < 0.0 )
        return 0;

    float height_length_squared = ((cylinder->height.n[0] * cylinder->height.n[0])
                                         + ((cylinder->height.n[1] * cylinder->height.n[1])
                                                 + (cylinder->height.n[2] * cylinder->height.n[2])));
    float distance_squared = ((dx * dx) + ((dz * dz) + (dy * dy)));
    if ( projection > height_length_squared
      || ((distance_squared * height_length_squared) - (projection * projection))
             >= (double)((cylinder->width * cylinder->width) * height_length_squared) )
    {
        return 0;
    }

    if ( height_length_squared <= 0.0 )
    {
        plane->n.n[0] = dx;
        plane->n.n[1] = dy;
        plane->n.n[2] = dz;
    }
    else
    {
        float parameter = (((cylinder->height.n[2] * dz)
                                        + ((cylinder->height.n[0] * dx) + (cylinder->height.n[1] * dy)))
                                / height_length_squared);
        plane->n.n[0] = dx - (cylinder->height.n[0] * parameter);
        plane->n.n[1] = dy - (cylinder->height.n[1] * parameter);
        plane->n.n[2] = dz - (cylinder->height.n[2] * parameter);
    }

    float length = normalize3d(&plane->n);
    if ( length == 0.0 )
    {
        plane->n.n[0] = 0.0;
        plane->n.n[1] = 0.0;
        plane->n.n[2] = 1.0;
    }
    plane->d = ((plane->n.n[0] * cylinder->base.n[0])
                            + ((plane->n.n[2] * cylinder->base.n[2])
                                    + (plane->n.n[1] * cylinder->base.n[1])))
                    + cylinder->width;
    *depth = cylinder->width - length;
    return 1;
}
