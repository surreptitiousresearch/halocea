/* angular_accelerate_to_position @ 0x83700358 — drive a unit facing vector `position` toward
 * `position_desired` while respecting a maximum angular velocity and a maximum angular acceleration. It
 * computes the angle between the two unit vectors, the minimum-energy slew speed sqrt(2*accel*angle)
 * (capped at the velocity maximum), the rotation axis (normalized cross product), and the target angular
 * velocity. The current angular velocity is moved toward that target by at most `accel` per call; once
 * close enough it snaps and, when the residual is negligible, jumps `position` straight to the desired
 * vector. Finally `position` is rotated about the (normalized) angular velocity by its magnitude and
 * renormalized.
 *
 * DEVIATION: the decompiler emitted "local variable allocation has failed" and mislabeled the two limit
 * arguments and the acos input/dot-product clamp (uninitialized fp reads). Reconstructed from disasm
 * 0x83700358-0x83700470: f30 = angular_velocity_magnitude_maximum (arg4), f31 =
 * angular_acceleration_magnitude_maximum (arg5); the clamped acos input is position·position_desired. */

#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"
extern double acos(double x);
extern double cos(double x);
extern double sin(double x);


extern float normalize3d(real_vector3d *v);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void angular_accelerate_to_position(
    real_vector3d *position,
    const real_vector3d *position_desired,
    real_vector3d *angular_velocity,
    float angular_velocity_magnitude_maximum,
    float angular_acceleration_magnitude_maximum)
{
    if ( angular_acceleration_magnitude_maximum <= 0.0f && angular_velocity_magnitude_maximum <= 0.0f )
    {
        *angular_velocity = *global_zero_vector3d;
        *position = *position_desired;
        return;
    }

    char snapped = 0;

    float dot = ((position_desired->n[1] * position->n[1])
        + ((position_desired->n[2] * position->n[2])
                + (position->n[0] * position_desired->n[0])));
    if ( dot < -1.0f )
        dot = -1.0f;
    else if ( dot > 1.0f )
        dot = 1.0f;

    float angle = (float)acos(dot);
    float two_alpha = ((angle * angular_acceleration_magnitude_maximum) * 2.0f);
    float speed;
    if ( two_alpha < (angular_velocity_magnitude_maximum * angular_velocity_magnitude_maximum) )
        speed = __fsqrts(two_alpha);
    else
        speed = angular_velocity_magnitude_maximum;

    float axis_x = ((position_desired->n[2] * position->n[1]) - (position_desired->n[1] * position->n[2]));
    float axis_y = ((position_desired->n[0] * position->n[2]) - (position_desired->n[2] * position->n[0]));
    float axis_z = ((position_desired->n[1] * position->n[0]) - (position_desired->n[0] * position->n[1]));
    float axis_length = __fsqrts(((axis_x * axis_x)
        + ((axis_z * axis_z) + (axis_y * axis_y))));
    if ( __fabs(axis_length) >= 0.000099999997f )
    {
        float inverse = (1.0f / axis_length);
        axis_x = (inverse * axis_x);
        axis_y = (axis_y * inverse);
        axis_z = (axis_z * inverse);
    }

    real_vector3d target_velocity;
    float velocity_x = angular_velocity->n[0];
    float velocity_y = angular_velocity->n[1];
    float velocity_z = angular_velocity->n[2];
    target_velocity.n[0] = (axis_x * speed);
    target_velocity.n[1] = (axis_y * speed);
    target_velocity.n[2] = (axis_z * speed);

    float delta_x = (target_velocity.n[0] - velocity_x);
    float delta_y = (target_velocity.n[1] - velocity_y);
    float delta_z = (target_velocity.n[2] - velocity_z);
    float delta_magnitude_squared = ((delta_x * delta_x)
        + ((delta_z * delta_z) + (delta_y * delta_y)));

    if ( delta_magnitude_squared >= (angular_acceleration_magnitude_maximum * angular_acceleration_magnitude_maximum) )
    {
        float delta_magnitude = __fsqrts(delta_magnitude_squared);
        float scale = (angular_acceleration_magnitude_maximum / delta_magnitude);
        angular_velocity->n[0] = ((delta_x * scale) + velocity_x);
        angular_velocity->n[1] = ((delta_y * scale) + velocity_y);
        angular_velocity->n[2] = ((delta_z * scale) + velocity_z);
    }
    else if ( speed >= 0.0000010000001f )
    {
        angular_velocity->n[0] = target_velocity.n[0];
        angular_velocity->n[1] = target_velocity.n[1];
        angular_velocity->n[2] = target_velocity.n[2];
    }
    else
    {
        snapped = 1;
        *angular_velocity = *global_zero_vector3d;
        position->n[0] = position_desired->n[0];
        position->n[1] = position_desired->n[1];
        position->n[2] = position_desired->n[2];
    }

    if ( !snapped )
    {
        real_vector3d rotation_axis;
        rotation_axis.n[0] = angular_velocity->n[0];
        rotation_axis.n[1] = angular_velocity->n[1];
        rotation_axis.n[2] = angular_velocity->n[2];
        float rotation_angle = normalize3d(&rotation_axis);
        if ( rotation_angle != 0.0f )
        {
            /* attested void: rotate_vector_about_axis rotates position in place (old return was r3=arg) */
            rotate_vector_about_axis(position, &rotation_axis,
                (float)sin(rotation_angle), (float)cos(rotation_angle));
            normalize3d(position);
        }
    }
}
