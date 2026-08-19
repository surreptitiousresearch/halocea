/* rotate_vectors3d_by_angular_velocity @0x837BFA98 — rotate a forward/up basis by one tick's worth of
 * angular velocity (axis = normalized angular_velocity, angle = its magnitude in radians). If the angular
 * velocity is negligible (magnitude < 0.0001), the basis passes through unrotated. Otherwise builds a
 * rotation matrix from the axis/angle, transforms both vectors, re-normalizes rotated_forward, then
 * re-orthogonalizes rotated_up against it (Gram-Schmidt: subtract the component of rotated_up along
 * rotated_forward) before re-normalizing rotated_up too.
 *
 * DEVIATION: the DB prototype confirms only 5 real parameters — the raw decompile's `a6` is a phantom
 * (Hex-Rays misreading a local double copy of the magnitude passed to `cos`/`sin`). The decompiler's
 * `magnitude == 0.0` term (only reachable after already establishing `fabs(magnitude) >= 0.0001`, via
 * short-circuit `||`) is dead — reproduced by simply skipping it. */

#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

extern float __fsqrts(float x);
extern double __fabs(double x);
extern double cos(double x);
extern double sin(double x);

extern void matrix4x3_rotation_from_axis_and_angle(real_matrix4x3 *matrix, const real_vector3d *axis, float sine, float cosine);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern float normalize3d(real_vector3d *v);

void rotate_vectors3d_by_angular_velocity(const real_vector3d *forward, const real_vector3d *up,
        const real_vector3d *angular_velocity, real_vector3d *rotated_forward, real_vector3d *rotated_up)
{
    double magnitude = __fsqrts((angular_velocity->n[2] * angular_velocity->n[2])
            + (angular_velocity->n[1] * angular_velocity->n[1])
            + (angular_velocity->n[0] * angular_velocity->n[0]));

    if ( __fabs(magnitude) < 0.0001 )
    {
        *rotated_forward = *forward;
        *rotated_up = *up;
        return;
    }

    real_vector3d axis;
    axis.n[0] = angular_velocity->n[0] * (1.0f / (float)magnitude);
    axis.n[1] = angular_velocity->n[1] * (1.0f / (float)magnitude);
    axis.n[2] = angular_velocity->n[2] * (1.0f / (float)magnitude);

    float cosine = (float)cos(magnitude);
    float sine = (float)sin(magnitude);

    real_matrix4x3 rotation;
    matrix4x3_rotation_from_axis_and_angle(&rotation, &axis, sine, cosine);
    matrix4x3_transform_vector(&rotation, forward, rotated_forward);
    matrix4x3_transform_vector(&rotation, up, rotated_up);
    normalize3d(rotated_forward);

    float dot = -(rotated_forward->n[1] * rotated_up->n[1] + rotated_up->n[0] * rotated_forward->n[0]
            + rotated_up->n[2] * rotated_forward->n[2]);

    rotated_up->n[0] = rotated_forward->n[0] * dot + rotated_up->n[0];
    rotated_up->n[1] = rotated_forward->n[1] * dot + rotated_up->n[1];
    rotated_up->n[2] = rotated_forward->n[2] * dot + rotated_up->n[2];
    normalize3d(rotated_up);
}
