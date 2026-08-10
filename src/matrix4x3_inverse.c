/* matrix4x3_inverse @ 0x837055F0 — invert an affine 4x3 matrix (uniform scale + rotation +
 * translation). The inverse scale is 1/scale, the inverse rotation is the transpose, and the inverse
 * translation is the transpose-rotated negative translation divided by scale. A zero-scale matrix
 * inverts to all-zero. */

#include <string.h>
#include "headers/real_matrix4x3.h"


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
            inv_scale = 1.0f / scale;
            tx = inv_scale * tx;
            ty = inv_scale * ty;
            tz = inv_scale * tz;
        }
        result->scale = inv_scale;

        /* DEVIATION: the source elements are held in locals across the stores, mirroring the
         * binary's FPR schedule. Every source offset is loaded before the store that overwrites
         * it — L/S 0x04 83705644/8370564C, 0x14 83705654/83705658, 0x24 8370565C/83705660,
         * 0x10 83705664/83705674, 0x08 8370566C/83705678, 0x0C 8370567C/83705690,
         * 0x1C 83705684/8370568C, 0x20 83705698/837056B8, 0x18 837056A4/837056C4 — so the
         * routine is alias-safe as built, and update_human_plane_physics @0x83761378 calls it
         * with r3 == r4. A statement-order transcription reads n[1][0]/n[2][0]/n[2][1]/n[0][2]/
         * n[1][2] back after those offsets were stored and corrupts the in-place case. */
        float m00 = matrix->n[0][0];  /* f11 @83705644 */
        float m11 = matrix->n[1][1];  /* f10 @83705654 */
        float m22 = matrix->n[2][2];  /* f9  @8370565C */
        float m10 = matrix->n[1][0];  /* f8  @83705664 */
        float m01 = matrix->n[0][1];  /* f7  @8370566C */
        float m02 = matrix->n[0][2];  /* f3  @8370567C */
        float m20 = matrix->n[2][0];  /* f4  @83705684 */
        float m21 = matrix->n[2][1];  /* f10 @83705698 */
        float m12 = matrix->n[1][2];  /* f9  @837056A4 */

        /* rotation = transpose of the input rotation */
        result->n[0][0] = m00;
        result->n[1][1] = m11;
        result->n[2][2] = m22;
        result->n[1][0] = m01;
        result->n[0][1] = m10;
        result->n[2][0] = m02;
        result->n[0][2] = m20;
        result->n[2][1] = m12;
        result->n[1][2] = m21;

        /* translation = transpose-rotation applied to the scaled negative translation.
         * DEVIATION: the n[3][2] row-dot takes n[2][1] (lfs f10, 0x20(r3) @83705698, consumed by
         * the fmadds @837056D4), not n[1][2] — the old spelling was only correct by accident, and
         * only when the caller aliased. */
        result->n[3][0] = m01 * ty + (m00 * tx + m02 * tz);
        result->n[3][1] = ty * m11 + (m10 * tx + m12 * tz);
        result->n[3][2] = m21 * ty + (m20 * tx + m22 * tz);
    }
}
