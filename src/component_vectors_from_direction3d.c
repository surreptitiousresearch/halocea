/* component_vectors_from_direction3d @0x836FCDF0 — decompose `vector` into its component parallel to
 * `direction` (the projection, direction need not be unit length — the dot product is normalized by
 * direction's squared magnitude) and the perpendicular remainder. A zero-length direction yields a zero
 * parallel part and passes the vector through as perpendicular. */

#include "headers/real_vector3d.h"

void component_vectors_from_direction3d(const real_vector3d *vector, const real_vector3d *direction,
        real_vector3d *parallel, real_vector3d *perpendicular)
{
    float direction_magnitude_squared = direction->n[2] * direction->n[2]
                                      + direction->n[0] * direction->n[0]
                                      + direction->n[1] * direction->n[1];
    if ( direction_magnitude_squared == 0.0f )
    {
        parallel->n[0] = 0.0f;
        parallel->n[1] = 0.0f;
        parallel->n[2] = 0.0f;
        *perpendicular = *vector;
        return;
    }

    float projection = (vector->n[1] * direction->n[1]
                      + vector->n[0] * direction->n[0]
                      + vector->n[2] * direction->n[2]) / direction_magnitude_squared;

    parallel->n[0] = direction->n[0] * projection;
    parallel->n[1] = direction->n[1] * projection;
    parallel->n[2] = direction->n[2] * projection;
    perpendicular->n[0] = vector->n[0] - parallel->n[0];
    perpendicular->n[1] = vector->n[1] - parallel->n[1];
    perpendicular->n[2] = vector->n[2] - parallel->n[2];
}
