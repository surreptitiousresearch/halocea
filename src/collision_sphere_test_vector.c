/* collision_sphere_test_vector @0x83805518 — cast the segment (point → point+vector) against a sphere feature.
 * If the origin is already inside, reports t=0. Otherwise solves the ray/sphere quadratic and reports the near
 * root if within the segment. On a hit writes the outward surface normal/plane at the contact. Returns TRUE. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_sphere.h"

extern float __fsqrts(float);
extern float normalize3d(real_vector3d *v);

/* was: int return — return byte-normalized (clrlwi r3,24) by sole caller collision_features_test_vector */
uint8_t collision_sphere_test_vector(const collision_sphere *sphere, const real_point3d *point,
                                 const real_vector3d *vector, float *t, real_plane3d *plane)
{
    float dx = sphere->center.n[0] - point->n[0];
    float dz = sphere->center.n[2] - point->n[2];
    float dy = sphere->center.n[1] - point->n[1];
    int hit = 0;

    float outside = -((sphere->radius * sphere->radius)
                                  - ((dy * dy) + ((dz * dz) + (dx * dx))));
    if ( outside <= 0.0 )
    {
        *t = 0.0;                       /* origin inside the sphere */
        hit = 1;
    }
    else
    {
        float projection = ((dy * vector->n[1])
                                 + ((vector->n[0] * dx) + (dz * vector->n[2])));
        if ( projection > 0.0 )
        {
            float vector_length_squared = ((vector->n[0] * vector->n[0])
                                                 + ((vector->n[1] * vector->n[1])
                                                         + (vector->n[2] * vector->n[2])));
            float discriminant = ((projection * projection)
                                       - (vector_length_squared * outside));
            if ( discriminant >= 0.0 )
            {
                float root = __fsqrts(discriminant);
                if ( (projection - root) <= (double)vector_length_squared )
                {
                    *t = ((projection - root) / vector_length_squared);
                    hit = 1;
                }
            }
        }
    }

    if ( hit )
    {
        plane->n.n[0] = (vector->n[0] * *t) - dx;
        plane->n.n[1] = (vector->n[1] * *t) - dy;
        plane->n.n[2] = (vector->n[2] * *t) - dz;
        if ( normalize3d(&plane->n) == 0.0 )
        {
            plane->n.n[0] = 0.0;
            plane->n.n[1] = 0.0;
            plane->n.n[2] = 1.0;
        }
        plane->d = ((plane->n.n[0] * sphere->center.n[0])
                                + ((sphere->center.n[1] * plane->n.n[1])
                                        + (sphere->center.n[2] * plane->n.n[2])))
                        + sphere->radius;
    }
    return hit;
}
