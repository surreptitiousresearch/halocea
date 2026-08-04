/* matrix4x3_identity @0x83705570 — set an affine matrix to the identity (scale 1, identity rotation, zero
 * translation). */

#include "headers/real_matrix4x3.h"

void matrix4x3_identity(real_matrix4x3 *matrix)
{
    matrix->scale = 1.0f;
    matrix->n[0][0] = 1.0f;
    matrix->n[0][1] = 0.0f;
    matrix->n[0][2] = 0.0f;
    matrix->n[1][0] = 0.0f;
    matrix->n[1][1] = 1.0f;
    matrix->n[1][2] = 0.0f;
    matrix->n[2][0] = 0.0f;
    matrix->n[2][1] = 0.0f;
    matrix->n[2][2] = 1.0f;
    matrix->n[3][0] = 0.0f;
    matrix->n[3][1] = 0.0f;
    matrix->n[3][2] = 0.0f;
}
