/* matrix3x3_determinant @0x83706918 */
#include "headers/real_matrix3x3.h"

float matrix3x3_determinant(const real_matrix3x3 *matrix)
{
    return matrix->n[0][0] * matrix->n[1][1] * matrix->n[2][2]
         - matrix->n[0][0] * matrix->n[1][2] * matrix->n[2][1]
         - matrix->n[0][1] * matrix->n[1][0] * matrix->n[2][2]
         + matrix->n[0][1] * matrix->n[1][2] * matrix->n[2][0]
         + matrix->n[0][2] * matrix->n[1][0] * matrix->n[2][1]
         - matrix->n[0][2] * matrix->n[1][1] * matrix->n[2][0];
}
