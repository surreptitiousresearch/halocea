/* matrix4x3_translation @0x83705738 — build a pure translation matrix: unit scale, identity rotation
 * (rows 0..2), translation in row 3. (Row aliases: row0=forward, row2=up, row3=position.) */

#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"

void matrix4x3_translation(real_matrix4x3 *matrix, const real_point3d *point)
{
    matrix->scale = 1.0f;
    matrix->n[0][0] = 1.0f;
    matrix->n[0][1] = 0.0f;
    matrix->n[0][2] = 0.0f;
    matrix->n[1][0] = 0.0f;
    matrix->n[1][1] = 1.0f;
    matrix->n[1][2] = 0.0f;
    matrix->n[2][0] = 0.0f;
    matrix->n[2][1] = 0.0f;
    matrix->n[2][2] = 1.0f;
    matrix->n[3][0] = point->n[0];
    matrix->n[3][1] = point->n[1];
    matrix->n[3][2] = point->n[2];
}
