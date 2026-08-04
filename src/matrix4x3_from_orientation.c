/* matrix4x3_from_orientation @0x83705EF0 — build an affine matrix from an orientation: rotation from the
 * quaternion, uniform scale and translation row copied from the orientation. */

#include "headers/real_matrix4x3.h"
#include "headers/real_orientation.h"

#include "headers/real_quaternion.h"
extern real_matrix4x3 *matrix4x3_rotation_from_quaternion(real_matrix4x3 *matrix, const real_quaternion *quaternion);

void matrix4x3_from_orientation(real_matrix4x3 *matrix, const real_orientation *orientation)
{
    matrix4x3_rotation_from_quaternion(matrix, &orientation->rotation);
    matrix->scale = orientation->scale;
    matrix->n[3][0] = orientation->translation.n[0];
    matrix->n[3][1] = orientation->translation.n[1];
    matrix->n[3][2] = orientation->translation.n[2];
}
