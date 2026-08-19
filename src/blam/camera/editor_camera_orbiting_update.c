/* editor_camera_orbiting_update @0x837C4250 — camera proc for the editor's unit-orbiting free camera:
 * always focuses on the local player's unit origin. While the control input is active, applies the
 * facing delta to the camera's orbit orientation, clamping pitch to +-0.4*pi (~72 degrees). Derives the
 * orbit distance from the mouse wheel delta, floored at 0.6. If the local player currently occupies a
 * unit, builds the forward/up vectors from the orbit orientation and reads the unit's velocity into
 * focus_velocity, marking the command valid (flags=1). Fixed field of view (~70 degrees) and a 0.5s
 * interpolation timer. */

#include <stdint.h>
#include "headers/flying_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/unit_camera_info.h"
#include "headers/observer_command_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void player_control_get_unit_camera_info(int16_t local_player_index, unit_camera_info *camera_info);
extern void director_inhibit_input(int16_t local_player_index);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);

void editor_camera_orbiting_update(flying_camera *camera, const camera_control *controls, observer_command *result)
{
    unit_camera_info camera_info;
    player_control_get_unit_camera_info(controls->local_player_index, &camera_info);

    result->___u1.__s0.focus_position = camera_info.unit_origin;

    if ( controls->active )
    {
        camera->orientation.n[0] += controls->facing_delta.n[0];

        float pitch = camera->orientation.n[1] + controls->facing_delta.n[1];
        if ( pitch < -1.2566371f )
            pitch = -1.2566371f;
        else if ( pitch > 1.2566371f )
            pitch = 1.2566371f;
        camera->orientation.n[1] = pitch;

        director_inhibit_input(controls->local_player_index);
    }

    float distance = -(controls->wheel_delta * 0.33333334f - camera->position.n[1]);
    if ( distance <= 0.6f )
        distance = 0.6f;
    camera->position.n[1] = distance;

    if ( camera_info.unit_index != -1 )
    {
        vector3d_from_euler_angles2d(&result->___u1.__s0.forward, &camera->orientation);
        observer_up_from_forward(&result->___u1.__s0.forward, &result->___u1.__s0.up);
        object_get_velocities(camera_info.unit_index, &result->focus_velocity, nullptr);
        result->flags = (1u << _observer_command_valid_bit);
    }

    result->___u1.__s0.focus_offset = *global_zero_vector3d;
    result->___u1.__s0.focus_distance = camera->position.n[1];
    result->___u1.__s0.field_of_view = 1.2217305f;
    result->timer = 0.5f;
}
