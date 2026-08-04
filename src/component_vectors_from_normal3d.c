/* component_vectors_from_normal3d @0x836FCD68 — decompose `vector` relative to a unit `normal` into its
 * parallel component (projection onto the normal) and perpendicular component (the remainder). Either
 * output may be null. */

#include "headers/real_vector3d.h"

void component_vectors_from_normal3d(const real_vector3d *vector, const real_vector3d *normal,
                                     real_vector3d *parallel, real_vector3d *perpendicular)
{
    float dot = ((normal->n[2] * vector->n[2])
                      + ((vector->n[0] * normal->n[0]) + (vector->n[1] * normal->n[1])));
    real_vector3d scratch;

    if ( !parallel )
        parallel = &scratch;

    parallel->n[0] = normal->n[0] * dot;
    parallel->n[1] = dot * normal->n[1];
    parallel->n[2] = normal->n[2] * dot;

    if ( perpendicular )
    {
        perpendicular->n[0] = vector->n[0] - parallel->n[0];
        perpendicular->n[1] = vector->n[1] - parallel->n[1];
        perpendicular->n[2] = vector->n[2] - parallel->n[2];
    }
}
