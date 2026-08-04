/* matrix4x3_inverse_transform_vector @ 0x837063D8 — transform a vector by the inverse of a 4x3 matrix
 * (divide by scale, then multiply by the rotation transpose; no translation). */

#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector,
                                                  real_vector3d *result)
{
    float x = vector->n[0];
    float y = vector->n[1];
    float z = vector->n[2];

    if ( matrix->scale != 1.0f )
    {
        float inv = 1.0f / matrix->scale;
        x = inv * vector->n[0];
        y = inv * vector->n[1];
        z = inv * vector->n[2];
    }

    result->n[0] = matrix->n[0][0] * x + (matrix->n[0][2] * z + matrix->n[0][1] * y);
    result->n[1] = matrix->n[1][0] * x + (matrix->n[1][2] * z + matrix->n[1][1] * y);
    result->n[2] = matrix->n[2][0] * x + (matrix->n[2][2] * z + matrix->n[2][1] * y);
    return result;
}
