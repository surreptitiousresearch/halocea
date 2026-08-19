/* matrix3x3_transpose @0x83706970 — transposes a 3x3 matrix into result; safe for result==matrix
 * (in-place transpose swaps the three off-diagonal pairs through temporaries instead of overwriting). */

#include "headers/real_matrix3x3.h"

real_matrix3x3 * matrix3x3_transpose(const real_matrix3x3 *matrix, real_matrix3x3 *result)
{
    if (matrix != result)
    {
        result->n[0][0] = matrix->n[0][0];
        result->n[0][1] = matrix->n[1][0];
        result->n[0][2] = matrix->n[2][0];
        result->n[1][0] = matrix->n[0][1];
        result->n[1][1] = matrix->n[1][1];
        result->n[1][2] = matrix->n[2][1];
        result->n[2][0] = matrix->n[0][2];
        result->n[2][1] = matrix->n[1][2];
        result->n[2][2] = matrix->n[2][2];
    }
    else
    {
        float n01 = matrix->n[0][1];
        result->n[0][1] = matrix->n[1][0];
        result->n[1][0] = n01;
        float n02 = matrix->n[0][2];
        result->n[0][2] = matrix->n[2][0];
        result->n[2][0] = n02;
        float n12 = matrix->n[1][2];
        result->n[1][2] = matrix->n[2][1];
        result->n[2][1] = n12;
    }
    return result;
}
