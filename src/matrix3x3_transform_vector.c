/* matrix3x3_transform_vector @0x83706E50 — transform `vector` by a 3x3 matrix into `result` (row-vector *
 * matrix). Safe for in-place use (vector == result): the input is fully copied to a stack scratch buffer
 * first, since the component writes below would otherwise clobber it mid-computation.
 *
 * DEVIATION: the decompiler only showed the n[2] component being saved to the scratch buffer before the
 * `vector = back_chain` reassignment; disasm shows all three components (n[0]/n[1]/n[2]) are actually saved —
 * the decompiler dropped 2 of the 3 stores, the same class of missed-instruction bug seen elsewhere in this
 * corpus for repeated/unrolled sequences. */

#include "headers/real_matrix3x3.h"
#include "headers/real_vector3d.h"

real_vector3d *matrix3x3_transform_vector(const real_matrix3x3 *matrix, const real_vector3d *vector, real_vector3d *result)
{
    real_vector3d scratch;
    if ( vector == result )
    {
        scratch = *vector;
        vector = &scratch;
    }

    result->n[0] = matrix->n[0][0] * vector->n[0] + (matrix->n[2][0] * vector->n[2] + matrix->n[1][0] * vector->n[1]);
    result->n[1] = matrix->n[1][1] * vector->n[1] + (matrix->n[2][1] * vector->n[2] + matrix->n[0][1] * vector->n[0]);
    result->n[2] = matrix->n[1][2] * vector->n[1] + (matrix->n[2][2] * vector->n[2] + matrix->n[0][2] * vector->n[0]);

    return result;
}
