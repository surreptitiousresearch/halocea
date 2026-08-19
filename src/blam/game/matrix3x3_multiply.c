/* matrix3x3_multiply @0x83706C80 — result = a * b (row-vector convention: result->n[row] = a->n[row] * b).
 * Safe for result aliasing either input: the decompiler shows this as a raw 36-byte stack-scratch copy of
 * whichever operand aliases result; reproduced here as ordinary local copies of the same effect. */

#include "headers/real_matrix3x3.h"

real_matrix3x3 *matrix3x3_multiply(const real_matrix3x3 *a, const real_matrix3x3 *b, real_matrix3x3 *result)
{
    real_matrix3x3 a_copy, b_copy;

    if (a == result)
    {
        a_copy = *a;
        a = &a_copy;
    }
    if (b == result)
    {
        b_copy = *b;
        b = &b_copy;
    }

    result->n[0][0] = a->n[0][0] * b->n[0][0] + a->n[1][0] * b->n[0][1] + a->n[2][0] * b->n[0][2];
    result->n[0][1] = a->n[0][1] * b->n[0][0] + a->n[1][1] * b->n[0][1] + a->n[2][1] * b->n[0][2];
    result->n[0][2] = a->n[0][2] * b->n[0][0] + a->n[1][2] * b->n[0][1] + a->n[2][2] * b->n[0][2];
    result->n[1][0] = a->n[0][0] * b->n[1][0] + a->n[1][0] * b->n[1][1] + a->n[2][0] * b->n[1][2];
    result->n[1][1] = a->n[0][1] * b->n[1][0] + a->n[1][1] * b->n[1][1] + a->n[2][1] * b->n[1][2];
    result->n[1][2] = a->n[0][2] * b->n[1][0] + a->n[1][2] * b->n[1][1] + a->n[2][2] * b->n[1][2];
    result->n[2][0] = a->n[0][0] * b->n[2][0] + a->n[1][0] * b->n[2][1] + a->n[2][0] * b->n[2][2];
    result->n[2][1] = a->n[0][1] * b->n[2][0] + a->n[1][1] * b->n[2][1] + a->n[2][1] * b->n[2][2];
    result->n[2][2] = a->n[0][2] * b->n[2][0] + a->n[1][2] * b->n[2][1] + a->n[2][2] * b->n[2][2];
    return result;
}
