/* matrix3x3_rotation_to_quaternion @0x83706EF0 — classic Shepperd's-method matrix-to-quaternion
 * conversion: uses the trace when positive, otherwise pivots on whichever diagonal element is largest to
 * avoid the numerically unstable near-zero-trace case.
 *
 * DEVIATION: the decompiler rendered several `matrix->n[i][i]`-style diagonal reads as flattened
 * single-dimension subscripts (e.g. `matrix->n[0][4 * (v4 > v5)]`); disasm_range(0x83706F98, 0x83706FE0)
 * confirms these are `lfsx` loads at `matrix + 16*i`, i.e. plain `matrix->n[i][i]`. `next_field_0` (the
 * cyclic {1,2,0} index table) is a plain data array at 0x84177CE8, confirmed via disasm_at. */

#include <math.h>

#include "headers/real_matrix3x3.h"
#include "headers/real_quaternion.h"

real_quaternion *matrix3x3_rotation_to_quaternion(const real_matrix3x3 *matrix, real_quaternion *quaternion)
{
    static const int next_field_0[3] = { 1, 2, 0 };

    float trace = matrix->n[0][0] + matrix->n[1][1] + matrix->n[2][2];
    if (trace > 0.0f)
    {
        float root = sqrtf(trace + 1.0f);
        float half_inverse = 0.5f / root;
        quaternion->w = root * 0.5f;
        quaternion->v.n[0] = (matrix->n[2][1] - matrix->n[1][2]) * half_inverse;
        quaternion->v.n[1] = (matrix->n[0][2] - matrix->n[2][0]) * half_inverse;
        quaternion->v.n[2] = (matrix->n[1][0] - matrix->n[0][1]) * half_inverse;
    }
    else
    {
        int i = matrix->n[1][1] > matrix->n[0][0];
        if (matrix->n[2][2] > matrix->n[i][i])
            i = 2;
        int j = next_field_0[i];
        int k = next_field_0[j];

        float components[3];
        float root = sqrtf(matrix->n[i][i] - (matrix->n[k][k] + matrix->n[j][j]) + 1.0f);
        components[i] = root * 0.5f;
        float half_inverse = (root != 0.0f) ? 0.5f / root : root;
        components[j] = (matrix->n[i][j] + matrix->n[j][i]) * half_inverse;
        components[k] = (matrix->n[i][k] + matrix->n[k][i]) * half_inverse;

        quaternion->v.n[0] = components[0];
        quaternion->v.n[1] = components[1];
        quaternion->v.n[2] = components[2];
        quaternion->w = (matrix->n[k][j] - matrix->n[j][k]) * half_inverse;
    }
    return quaternion;
}
