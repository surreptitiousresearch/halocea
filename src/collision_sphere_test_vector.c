/* collision_sphere_test_vector @0x83805518 — cast the segment (point → point+vector) against a sphere feature.
 * If the origin is already inside, reports t=0. Otherwise solves the ray/sphere quadratic and reports the near
 * root if within the segment. On a hit writes the outward surface normal/plane at the contact. Returns TRUE. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/collision_sphere.h"
#include "headers/fused_math.h"

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

    /* DEVIATION: fused distance/radius test — fmuls @83805568, fmadds @8380556C/83805570, fnmsubs @83805574 */
    float distance_squared = fused_madd(dy, dy, fused_madd(dz, dz, dx * dx));
    float outside = fused_nmsub(sphere->radius, sphere->radius, distance_squared);
    if ( outside <= 0.0 )
    {
        *t = 0.0;                       /* origin inside the sphere */
        hit = 1;
    }
    else
    {
        /* DEVIATION: fused projection dot — fmuls @8380558C, fmadds @83805598/8380559C */
        float projection = fused_madd(dy, vector->n[1],
                               fused_madd(vector->n[0], dx, dz * vector->n[2]));
        if ( projection > 0.0 )
        {
            /* DEVIATION: fused length/discriminant — fmuls @838055AC, fmadds @838055B4/838055B8, fmsubs @838055C0 */
            float vector_length_squared = fused_madd(vector->n[0], vector->n[0],
                                              fused_madd(vector->n[1], vector->n[1],
                                                  vector->n[2] * vector->n[2]));
            float discriminant = fused_msub(projection, projection,
                                     vector_length_squared * outside);
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
        /* DEVIATION: fused plane-d dot — fmuls @83805654, fmadds @8380566C/83805670, fadds @83805674 */
        plane->d = fused_madd(plane->n.n[0], sphere->center.n[0],
                       fused_madd(sphere->center.n[1], plane->n.n[1],
                           sphere->center.n[2] * plane->n.n[2]))
                        + sphere->radius;
    }
    return hit;
}
