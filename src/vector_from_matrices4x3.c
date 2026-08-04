/* vector_from_matrices4x3 @0x8370710C — compute the rotation between two 4x3 transforms as a scaled
 * axis vector: form the relative transform a * inverse(b), convert its rotation to a quaternion, then
 * to angle-axis, and return the axis scaled by the angle (an exponential-map / rotation vector).
 *
 * DEVIATION: the axis-scaling multiplies are computed in floats but routed through doubles by the
 * soft-float ABI. */

#include "headers/real_matrix4x3.h"
#include "headers/real_quaternion.h"
#include "headers/real_vector3d.h"

extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern void matrix4x3_multiply_vmx(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void matrix4x3_rotation_to_quaternion(const real_matrix4x3 *matrix, real_quaternion *quaternion);
extern void quaternion_to_angle_and_vector(const real_quaternion *q, float *angle, real_vector3d *axis);

real_vector3d *vector_from_matrices4x3(const real_matrix4x3 *a, const real_matrix4x3 *b, real_vector3d *rotation)
{
    real_matrix4x3 inverse_b;
    real_matrix4x3 relative;
    real_quaternion relative_rotation;
    float angle[4];

    matrix4x3_inverse(b, &inverse_b);
    matrix4x3_multiply_vmx(a, &inverse_b, &relative);
    matrix4x3_rotation_to_quaternion(&relative, &relative_rotation);
    quaternion_to_angle_and_vector(&relative_rotation, angle, rotation);

    rotation->n[0] = rotation->n[0] * angle[0];
    rotation->n[1] = rotation->n[1] * angle[0];
    rotation->n[2] = rotation->n[2] * angle[0];
    return rotation;
}
