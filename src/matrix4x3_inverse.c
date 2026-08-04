/* matrix4x3_inverse @ 0x837055F0 — invert an affine 4x3 matrix (uniform scale + rotation +
 * translation). The inverse scale is 1/scale, the inverse rotation is the transpose, and the inverse
 * translation is the transpose-rotated negative translation divided by scale. A zero-scale matrix
 * inverts to all-zero. */

#include "headers/real_matrix4x3.h"

extern void *memset(void *dst, int value, unsigned int n);

void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result)
{
    float scale = matrix->scale;

    if ( scale == 0.0f )
    {
        memset(result, 0, sizeof(real_matrix4x3));
        return;
    }

    {
        float tx = -matrix->n[3][0];
        float ty = -matrix->n[3][1];
        float tz = -matrix->n[3][2];
        float inv_scale = 1.0f;

        if ( scale != 1.0f )
        {
            inv_scale = 1.0f / matrix->scale;
            tx = inv_scale * tx;
            ty = inv_scale * ty;
            tz = inv_scale * tz;
        }
        result->scale = inv_scale;

        /* rotation = transpose of the input rotation */
        result->n[0][0] = matrix->n[0][0];
        result->n[1][1] = matrix->n[1][1];
        result->n[2][2] = matrix->n[2][2];
        result->n[1][0] = matrix->n[0][1];
        result->n[0][1] = matrix->n[1][0];
        result->n[2][0] = matrix->n[0][2];
        result->n[0][2] = matrix->n[2][0];
        result->n[2][1] = matrix->n[1][2];
        result->n[1][2] = matrix->n[2][1];

        /* translation = transpose-rotation applied to the scaled negative translation */
        result->n[3][0] = result->n[1][0] * ty + (matrix->n[0][0] * tx + matrix->n[0][2] * tz);
        result->n[3][1] = ty * result->n[1][1] + (matrix->n[1][0] * tx + matrix->n[1][2] * tz);
        result->n[3][2] = matrix->n[1][2] * ty + (matrix->n[2][0] * tx + result->n[2][2] * tz);
    }
}
