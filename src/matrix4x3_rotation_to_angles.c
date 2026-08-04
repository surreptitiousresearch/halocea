/* matrix4x3_rotation_to_angles @0x83705A20 — extract yaw/pitch/roll from a rotation matrix. pitch = -asin(n[2][0]);
 * if the pitch is near +-90deg (cos near 0) the rotation is gimbal-locked: roll is set to 0 and yaw recovered from
 * the n[0][1]/n[1][1] terms, otherwise yaw and roll come from the remaining matrix entries divided by cos(pitch). */

#include <math.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_euler_angles3d.h"

void matrix4x3_rotation_to_angles(const real_matrix4x3 *matrix, real_euler_angles3d *angles)
{
    float negative_pitch = -asinf(matrix->n[2][0]);
    angles->__s1.pitch = negative_pitch;   /* union view: yaw/pitch/roll live in __s1 */
    float cos_pitch = cosf(negative_pitch);

    if ( cos_pitch <= 0.0001f )
    {
        angles->__s1.roll = 0.0f;
        angles->__s1.yaw = atan2f(matrix->n[0][1], matrix->n[1][1]);
    }
    else
    {
        float inverse_cos = 1.0f / cos_pitch;
        angles->__s1.roll = atan2f(-(matrix->n[2][1] * inverse_cos), matrix->n[2][2] * inverse_cos);
        angles->__s1.yaw = atan2f(-(matrix->n[1][0] * inverse_cos), matrix->n[0][0] * inverse_cos);
    }
}
