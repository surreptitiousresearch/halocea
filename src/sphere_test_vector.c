/* sphere_test_vector @0x837E0C00 — intersect the ray (point + t*vector) against a sphere (center, radius),
 * writing the near-hit fraction into *t_reference. Returns TRUE on a hit (t in [0,1]); if the ray origin is
 * already inside the sphere it reports a hit at t=0.
 *
 * Deviation: the DB 5-param prototype is authoritative. radius(f1) reserves a GPR (r4), so the pointer args land
 * in r3/r5/r6/r7 (center/point/vector/t_reference). Hex-Rays mis-shifted these into a phantom 6th arg; verified
 * via prologue/store disasm (origin read from r5, direction from r6, result stored to r7). */

#include <stdint.h>

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern float __fsqrts(float);

uint8_t sphere_test_vector(const real_point3d *center, float radius, const real_point3d *point,
                           const real_vector3d *vector, float *t_reference)
{
    float dx = center->n[0] - point->n[0];
    float dy = center->n[1] - point->n[1];
    float dz = center->n[2] - point->n[2];
    float outside = (((dx * dx) + ((dy * dy) + (dz * dz)))
                          - (radius * radius));
    if ( outside <= 0.0 )
    {
        *t_reference = 0.0;          /* origin inside the sphere */
        return 1;
    }

    float projection = ((dx * vector->n[0])
                             + ((dy * vector->n[1]) + (dz * vector->n[2])));
    if ( projection <= 0.0 )
        return 0;                    /* ray points away from the sphere */

    float vector_length_squared =
        ((vector->n[0] * vector->n[0])
              + ((vector->n[1] * vector->n[1]) + (vector->n[2] * vector->n[2])));
    float discriminant = ((projection * projection) - (vector_length_squared * outside));
    if ( discriminant < 0.0 )
        return 0;

    float t = ((projection - __fsqrts(discriminant)) / vector_length_squared);
    if ( t > 1.0 )
        return 0;

    *t_reference = t;
    return 1;
}
