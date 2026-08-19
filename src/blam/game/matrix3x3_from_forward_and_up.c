/* matrix3x3_from_forward_and_up @0x83706B38 — builds an orthonormal basis matrix from a forward and up
 * vector: forward and up are stored verbatim, and left is derived as their cross product (up x forward). */

#include "headers/real_matrix3x3.h"

real_matrix3x3 *matrix3x3_from_forward_and_up(real_matrix3x3 *matrix, const real_vector3d *forward,
        const real_vector3d *up)
{
    matrix->forward = *forward;
    matrix->left.i = forward->k * up->j - up->k * forward->j;
    matrix->left.k = up->i * forward->j - forward->i * up->j;
    matrix->left.j = up->k * forward->i - forward->k * up->i;
    matrix->up = *up;
    return matrix;
}
