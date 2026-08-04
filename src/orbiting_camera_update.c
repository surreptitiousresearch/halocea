/* orbiting_camera_update @ 0x837C3008 — camera proc that orbits the controlled unit. Look input spins
 * the orbit yaw and pitches it (clamped to +/-72 deg); the scroll wheel pushes the orbit distance in/out
 * (min 0.6). The focus sits at the unit origin (raised 0.52), the eye looks along the orbit orientation,
 * and the focus velocity tracks the unit. Uses a fixed FOV and a 0.5s interpolation timer. */

#include <stdint.h>
#include "headers/orbiting_camera.h"
#include "headers/unit_camera_info.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
extern void player_control_get_unit_camera_info(int16_t local_player_index, unit_camera_info *camera_info);
extern void director_inhibit_input(int16_t local_player_index);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);

void orbiting_camera_update(orbiting_camera *camera, const camera_control *controls, observer_command *result)
{
    unit_camera_info info;
    double pitch;
    double distance;

    player_control_get_unit_camera_info(controls->local_player_index, &info);
    result->focus_position.n[0] = info.unit_origin.n[0];
    result->focus_position.n[1] = info.unit_origin.n[1];
    result->focus_position.n[2] = info.unit_origin.n[2];

    if ( controls->active )
    {
        camera->orientation.n[0] = camera->orientation.n[0] - controls->facing_delta.n[0];
        pitch = (camera->orientation.n[1] - controls->facing_delta.n[1]);
        if ( pitch < -1.2566371 )      pitch = -1.2566371;
        else if ( pitch > 1.2566371 )  pitch = 1.2566371;
        camera->orientation.n[1] = pitch;
        director_inhibit_input(controls->local_player_index);
    }

    distance = -((controls->wheel_delta * 0.33333334f) - camera->distance);
    if ( distance <= 0.60000002 )
        distance = 0.60000002;
    camera->distance = distance;

    if ( info.unit_index != -1 )
    {
        vector3d_from_euler_angles2d(&result->forward, &camera->orientation);
        observer_up_from_forward(&result->forward, &result->up);
        object_get_velocities(info.unit_index, &result->focus_velocity, 0);
        result->focus_position.n[2] = result->focus_position.n[2] + 0.51999998f;
        result->flags = 1;
    }

    result->focus_offset = *global_zero_vector3d;
    result->focus_distance = camera->distance;
    result->field_of_view = 0.87266463f;
    result->timer = 0.5f;
}
