/* matrix4x3_to_point_and_vectors @0x83706088 — decompose a 4x3 matrix into its translation (row 3), forward
 * vector (row 0) and up vector (row 2). The third basis axis (row 1) is not returned. */

#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

void matrix4x3_to_point_and_vectors(const real_matrix4x3 *matrix, real_point3d *point,
                                    real_vector3d *forward, real_vector3d *up)
{
    forward->n[0] = matrix->n[0][0];
    forward->n[1] = matrix->n[0][1];
    forward->n[2] = matrix->n[0][2];
    up->n[0] = matrix->n[2][0];
    up->n[1] = matrix->n[2][1];
    up->n[2] = matrix->n[2][2];
    point->n[0] = matrix->n[3][0];
    point->n[1] = matrix->n[3][1];
    point->n[2] = matrix->n[3][2];
}
