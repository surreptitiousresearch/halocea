/* matrix4x3_multiply_cpp @0x83706558 — scalar (non-VMX) affine matrix product result = a * b. Each operand that
 * aliases the result is first copied to a local so the multiply is safe in place. The rotation block is the usual
 * 3x3 product; the translation row is b's translation rotated by a, scaled by a's scale, plus a's translation; the
 * combined uniform scale is the product of the two scales. Component grouping preserved from the decompiler. */

#include <string.h>
#include "headers/real_matrix4x3.h"

void matrix4x3_multiply_cpp(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result)
{
    real_matrix4x3 temp;
    if ( a == result )
    {
        memcpy(&temp, a, sizeof(real_matrix4x3));
        a = &temp;
    }
    if ( b == result )
    {
        memcpy(&temp, b, sizeof(real_matrix4x3));
        b = &temp;
    }

    result->n[0][0] = (b->n[0][2] * a->n[2][0]) + ((a->n[1][0] * b->n[0][1]) + (a->n[0][0] * b->n[0][0]));
    result->n[0][1] = (a->n[1][1] * b->n[0][1]) + ((a->n[2][1] * b->n[0][2]) + (a->n[0][1] * b->n[0][0]));
    result->n[0][2] = (a->n[1][2] * b->n[0][1]) + ((a->n[2][2] * b->n[0][2]) + (a->n[0][2] * b->n[0][0]));

    result->n[1][0] = (a->n[1][0] * b->n[1][1]) + ((a->n[0][0] * b->n[1][0]) + (b->n[1][2] * a->n[2][0]));
    result->n[1][1] = (a->n[0][1] * b->n[1][0]) + ((a->n[2][1] * b->n[1][2]) + (a->n[1][1] * b->n[1][1]));
    result->n[1][2] = (a->n[0][2] * b->n[1][0]) + ((a->n[2][2] * b->n[1][2]) + (a->n[1][2] * b->n[1][1]));

    result->n[2][0] = (a->n[1][0] * b->n[2][1]) + ((b->n[2][2] * a->n[2][0]) + (a->n[0][0] * b->n[2][0]));
    result->n[2][1] = (b->n[2][1] * a->n[1][1]) + ((a->n[0][1] * b->n[2][0]) + (b->n[2][2] * a->n[2][1]));
    result->n[2][2] = (b->n[2][1] * a->n[1][2]) + ((a->n[0][2] * b->n[2][0]) + (b->n[2][2] * a->n[2][2]));

    result->n[3][0] = (((b->n[3][2] * a->n[2][0]) + ((a->n[0][0] * b->n[3][0]) + (a->n[1][0] * b->n[3][1]))) * a->scale)
                    + a->n[3][0];
    result->n[3][1] = (((b->n[3][0] * a->n[0][1]) + ((a->n[2][1] * b->n[3][2]) + (b->n[3][1] * a->n[1][1]))) * a->scale)
                    + a->n[3][1];
    result->n[3][2] = (((b->n[3][0] * a->n[0][2]) + ((a->n[2][2] * b->n[3][2]) + (b->n[3][1] * a->n[1][2]))) * a->scale)
                    + a->n[3][2];

    result->scale = a->scale * b->scale;
}
