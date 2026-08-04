/* matrix3x3_inverse @0x83706A00 — invert a 3x3 matrix given its precomputed determinant, via the
 * cofactor/adjugate formula (each output entry is a 2x2 minor of the wraparound-adjacent rows/columns,
 * divided by determinant). Supports in-place use (matrix == result) by snapshotting the input into a local
 * copy first.
 *
 * DEVIATION: the decompiler renders this with an extra phantom 3rd parameter ("result", never referenced
 * in the body) alongside the real output pointer ("a4"); the DB's 3-parameter prototype
 * (matrix, determinant, result) is ground truth — "a4" is that real `result` output pointer. */

#include "headers/real_matrix3x3.h"

real_matrix3x3 * matrix3x3_inverse(const real_matrix3x3 *matrix, float determinant, real_matrix3x3 *result)
{
    real_matrix3x3 local_copy;
    if ( matrix == result )
    {
        local_copy = *matrix;
        matrix = &local_copy;
    }

    float inverse_determinant = 1.0f / determinant;

    /* Output is transposed relative to the input row/col basis used below (adjugate = transpose(cofactor)):
     * result->n[col][row] is written for each (row, col) pair of the input matrix's minor. */
    for ( __int16 row = 0; row < 3; row++ )
    {
        __int16 next_row = (row >= 2) ? 0 : row + 1;
        __int16 prev_row = (row <= 0) ? 2 : row - 1;

        for ( __int16 col = 0; col < 3; col++ )
        {
            __int16 next_col = (col >= 2) ? 0 : col + 1;
            __int16 prev_col = (col <= 0) ? 2 : col - 1;

            result->n[col][row] = (matrix->n[next_row][next_col] * matrix->n[prev_row][prev_col]
                                  - matrix->n[next_row][prev_col] * matrix->n[prev_row][next_col])
                                 * inverse_determinant;
        }
    }

    return result;
}
