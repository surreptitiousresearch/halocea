/* matrix4x3_rotation_from_vectors @0x83705878 — build a rotation matrix from a forward and up vector. Row 0
 * is forward, row 2 is up, and row 1 is the third basis axis (up x forward). Unit scale, zero translation.
 * (Row aliases: row0=forward, row2=up, row3=position.) */

#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"

void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward,
                                     const real_vector3d *up)
{
    matrix->scale = 1.0f;
    matrix->n[0][0] = forward->n[0];
    matrix->n[0][1] = forward->n[1];
    matrix->n[0][2] = forward->n[2];

    matrix->n[1][1] = (up->n[2] * forward->n[0]) - (forward->n[2] * up->n[0]);
    matrix->n[1][0] = (forward->n[2] * up->n[1]) - (up->n[2] * forward->n[1]);
    matrix->n[1][2] = (up->n[0] * forward->n[1]) - (forward->n[0] * up->n[1]);

    matrix->n[2][0] = up->n[0];
    matrix->n[2][1] = up->n[1];
    matrix->n[2][2] = up->n[2];

    matrix->n[3][0] = 0.0f;
    matrix->n[3][1] = 0.0f;
    matrix->n[3][2] = 0.0f;
}
