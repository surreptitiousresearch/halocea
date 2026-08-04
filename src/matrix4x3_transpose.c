/* matrix4x3_transpose @0x837055B8 — transpose the 3x3 rotation part in place (scale and translation untouched). */

#include "headers/real_matrix4x3.h"

void matrix4x3_transpose(real_matrix4x3 *matrix)
{
    float n01 = matrix->n[0][1];
    float n20 = matrix->n[2][0];
    float n02 = matrix->n[0][2];
    float n21 = matrix->n[2][1];
    float n12 = matrix->n[1][2];

    matrix->n[0][1] = matrix->n[1][0];
    matrix->n[0][2] = n20;
    matrix->n[1][0] = n01;
    matrix->n[2][0] = n02;
    matrix->n[2][1] = n12;
    matrix->n[1][2] = n21;
}
