/* flying_camera_update @ 0x837C2E08 — camera proc for the free-flying debug camera. Look input rotates
 * the camera yaw/pitch (pitch clamped to +/-89.8 deg) and rolls it; the forward/up vectors are rebuilt
 * from the euler orientation and rolled about the forward axis. Movement input translates the camera
 * along its forward axis and its right axis (right = up x forward), scaled by the global speed_scale.
 * The result focuses on the camera position with no focus offset and the camera's own FOV. */

#include "headers/flying_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern double cos(double x);
extern double sin(double x);

void flying_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result)
{
    double pitch;
    double cos_roll, sin_roll;

    if ( controls->active )
    {
        camera->orientation.n[0] = controls->facing_delta.n[0] + camera->orientation.n[0];
        pitch = (controls->facing_delta.n[1] + camera->orientation.n[1]);
        if ( pitch < -1.5676548 )      pitch = -1.5676548;
        else if ( pitch > 1.5676548 )  pitch = 1.5676548;
        camera->orientation.n[1] = pitch;
        camera->roll = controls->facing_delta.n[2] + camera->roll;
    }

    result->timer = 0.30000001f;
    vector3d_from_euler_angles2d(&result->___u1.__s0.forward, &camera->orientation);
    observer_up_from_forward(&result->___u1.__s0.forward, &result->___u1.__s0.up);
    cos_roll = cos(camera->roll);
    sin_roll = sin(camera->roll);
    rotate_vector_about_axis(&result->___u1.__s0.up, &result->___u1.__s0.forward, (float)sin_roll, (float)cos_roll);

    if ( controls->active )
    {
        /* translate along forward (position_delta.x) and right = up x forward (position_delta.y) */
        float right_y = ((result->___u1.__s0.up.n[0] * result->___u1.__s0.forward.n[2])
                              - (result->___u1.__s0.forward.n[0] * result->___u1.__s0.up.n[2]));
        camera->position.n[0] =
            -(((controls->position_delta.n[1] * speed_scale)
                           * ((result->___u1.__s0.up.n[2] * result->___u1.__s0.forward.n[1])
                                   - (result->___u1.__s0.up.n[1] * result->___u1.__s0.forward.n[2])))
                   - (((result->___u1.__s0.forward.n[0] * controls->position_delta.n[0]) * speed_scale)
                           + camera->position.n[0]));
        camera->position.n[2] =
            -(((controls->position_delta.n[1] * speed_scale)
                           * ((result->___u1.__s0.forward.n[0] * result->___u1.__s0.up.n[1])
                                   - (result->___u1.__s0.up.n[0] * result->___u1.__s0.forward.n[1])))
                   - (((result->___u1.__s0.forward.n[2] * controls->position_delta.n[0]) * speed_scale)
                           + camera->position.n[2]));
        camera->position.n[1] =
            -(((controls->position_delta.n[1] * speed_scale) * right_y)
                   - (((result->___u1.__s0.forward.n[1] * controls->position_delta.n[0]) * speed_scale)
                           + camera->position.n[1]));
    }

    result->___u1.__s0.focus_position.n[0] = camera->position.n[0];
    result->___u1.__s0.focus_position.n[1] = camera->position.n[1];
    result->___u1.__s0.focus_position.n[2] = camera->position.n[2];
    result->___u1.__s0.focus_offset = *global_zero_vector3d;
    result->___u1.__s0.focus_distance = 0.0f;
    result->___u1.__s0.field_of_view = camera->field_of_view;
    result->flags = 1;
}
