/* matrix4x3_inverse_transform_normal @0x83706468 — transform a normal by the inverse rotation (the matrix's rows
 * dotted with the normal). Source components are captured first so result may alias normal. */

#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"

real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result)
{
    float nx = normal->n[0];
    float ny = normal->n[1];
    float nz = normal->n[2];

    result->n[0] = (matrix->n[0][0] * nx) + ((matrix->n[0][2] * nz) + (matrix->n[0][1] * ny));
    result->n[1] = (matrix->n[1][0] * nx) + ((matrix->n[1][2] * nz) + (matrix->n[1][1] * ny));
    result->n[2] = (matrix->n[2][0] * nx) + ((matrix->n[2][2] * nz) + (matrix->n[2][1] * ny));
    return result;
}
