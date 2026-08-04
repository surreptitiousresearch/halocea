/* matrix4x3_transform_point @ 0x837060D8 — transform a point by an affine 4x3 matrix (uniform scale
 * + rotation + translation): scale, rotate, then add the translation row. */

#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"

real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point,
                                        real_point3d *result)
{
    float x = point->n[0];
    float y = point->n[1];
    float z = point->n[2];

    if ( matrix->scale != 1.0f )
    {
        x = matrix->scale * point->n[0];
        y = matrix->scale * point->n[1];
        z = matrix->scale * point->n[2];
    }

    result->n[0] = (matrix->n[0][0] * x + (matrix->n[2][0] * z + matrix->n[1][0] * y)) + matrix->n[3][0];
    result->n[1] = (matrix->n[0][1] * x + (matrix->n[2][1] * z + matrix->n[1][1] * y)) + matrix->n[3][1];
    result->n[2] = (matrix->n[0][2] * x + (matrix->n[2][2] * z + matrix->n[1][2] * y)) + matrix->n[3][2];
    return result;
}
