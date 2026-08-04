/* matrix4x3_scale @0x837056F0 — set an affine matrix to a uniform scale (identity rotation, zero translation). */

#include "headers/real_matrix4x3.h"

void matrix4x3_scale(real_matrix4x3 *matrix, float scale)
{
    matrix->scale = scale;
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
