/* matrix4x3_multiply_vmx @0x837067C8 — concatenate two affine 4x3 transforms: result = a then b
 * (a point transformed by result == that point transformed by a, then by b). Computes
 *     result.scale       = a.scale * b.scale
 *     result.rotation    = a.rotation * b.rotation        (rows 0..2, direction vectors: no scale/offset)
 *     result.translation = transform(a.translation by b)  (row 3: rotate by b, scale by b.scale, add b.t)
 *
 * deviation: the original is hand-written VMX/AltiVec inline assembly (load-left/right, vmaddfp, vperm) that
 * loads both operands into vector registers up front. This is the equivalent scalar form; it accumulates
 * into a local so an aliasing result (the common in-place result == b case) stays correct. */

#include "headers/real_matrix4x3.h"

void matrix4x3_multiply_vmx(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result)
{
    real_matrix4x3 out;
    int j;

    out.scale = a->scale * b->scale;

    /* rotation rows (direction vectors): out.row_i = a.row_i * b.rotation */
    for ( j = 0; j < 3; ++j )
    {
        out.n[0][j] = a->n[0][0] * b->n[0][j] + a->n[0][1] * b->n[1][j] + a->n[0][2] * b->n[2][j];
        out.n[1][j] = a->n[1][0] * b->n[0][j] + a->n[1][1] * b->n[1][j] + a->n[1][2] * b->n[2][j];
        out.n[2][j] = a->n[2][0] * b->n[0][j] + a->n[2][1] * b->n[1][j] + a->n[2][2] * b->n[2][j];
    }

    /* translation row: rotate a.translation by b, apply b's scale, then add b's translation */
    for ( j = 0; j < 3; ++j )
        out.n[3][j] = (a->n[3][0] * b->n[0][j] + a->n[3][1] * b->n[1][j] + a->n[3][2] * b->n[2][j])
                      * b->scale + b->n[3][j];

    *result = out;
}
