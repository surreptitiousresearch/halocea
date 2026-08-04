/* matrix4x3_transform_normal @ 0x83706210 — rotate a direction vector by a 4x3 matrix (rotation
 * only; no scale or translation applied). */

#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal,
                                          real_vector3d *result)
{
    float x = normal->n[0];
    float y = normal->n[1];
    float z = normal->n[2];

    result->n[0] = matrix->n[0][0] * x + (matrix->n[2][0] * z + matrix->n[1][0] * y);
    result->n[1] = matrix->n[0][1] * x + (matrix->n[2][1] * z + matrix->n[1][1] * y);
    result->n[2] = matrix->n[0][2] * x + (matrix->n[2][2] * z + matrix->n[1][2] * y);
    return result;
}
