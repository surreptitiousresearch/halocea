/* matrix4x3_from_point_and_vectors @ 0x83705E00 — build an orthonormal 4x3 matrix (scale 1) from a
 * position and a forward/up basis. Row 0 = forward, row 2 = up, row 1 = up x forward (the right
 * vector), row 3 = position. The forward/up are assumed already unit and orthogonal. */

#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point,
                                      const real_vector3d *forward, const real_vector3d *up)
{
    matrix->scale = 1.0f;

    matrix->n[0][0] = forward->n[0];
    matrix->n[0][1] = forward->n[1];
    matrix->n[0][2] = forward->n[2];

    /* right = up x forward */
    matrix->n[1][1] = up->n[2] * forward->n[0] - forward->n[2] * up->n[0];
    matrix->n[1][0] = forward->n[2] * up->n[1] - up->n[2] * forward->n[1];
    matrix->n[1][2] = up->n[0] * forward->n[1] - forward->n[0] * up->n[1];

    matrix->n[2][0] = up->n[0];
    matrix->n[2][1] = up->n[1];
    matrix->n[2][2] = up->n[2];

    matrix->n[3][0] = 0.0f;
    matrix->n[3][1] = 0.0f;
    matrix->n[3][2] = 0.0f;
    matrix->n[3][0] = point->n[0];
    matrix->n[3][1] = point->n[1];
    matrix->n[3][2] = point->n[2];
}
