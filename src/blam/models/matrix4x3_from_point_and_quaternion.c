/* matrix4x3_from_point_and_quaternion @0x83705EA8 — build an affine 4x3 transform whose 3x3 rotation comes from
 * a quaternion and whose translation row (n[3]) is a point. The rotation helper fills scale + rows 0..2; this
 * routine then copies the point into the translation row at offset 0x28 (n[3][0..2]). */

#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_quaternion.h"

extern real_matrix4x3 *matrix4x3_rotation_from_quaternion(real_matrix4x3 *matrix, const real_quaternion *quaternion);

void matrix4x3_from_point_and_quaternion(real_matrix4x3 *matrix, const real_point3d *point, const real_quaternion *quaternion)
{
    matrix4x3_rotation_from_quaternion(matrix, quaternion);
    matrix->n[3][0] = point->x;
    matrix->n[3][1] = point->y;
    matrix->n[3][2] = point->z;
}
