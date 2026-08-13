/* first_person_camera_update @ 0x837C2610 — the camera proc for the standard first-person view. It
 * builds the observer command directly from the controlled unit's eye position and facing direction,
 * sets the field of view from the player's control state (kicking off a short FOV interpolation when
 * the FOV changes), and marks the orientation/position/flags as a hard snap so the observer follows
 * the unit exactly (no smoothing) — unless this is the first frame on this proc, in which case the FOV
 * timer is also zeroed for an instant cut. */

#include <stdint.h>
#include "headers/first_person_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_vector3d.h"
#include "headers/observer_time_flags.h"

/* the last camera proc that ran this frame — a director.camera_proc value, typed as such so the
 * identity test below needs no cast */
extern void (*g_LastCameraUpdateProc)(void *camera_data, const camera_control *controls, observer_command *result);

extern int player_control_get_unit_index(int16_t local_player_index);
extern real_vector3d *player_control_get_facing_direction(int16_t local_player_index, real_vector3d *direction);
extern void first_person_camera_for_unit_and_vector(int unit_index, const real_vector3d *forward, observer_command *result);
extern float player_control_get_field_of_view(int16_t local_player_index);

void first_person_camera_update(first_person_camera *camera, const camera_control *controls,
                                observer_command *result)
{
    int unit_index;
    real_vector3d facing[5];
    float field_of_view;

    unit_index = player_control_get_unit_index(controls->local_player_index);
    player_control_get_facing_direction(controls->local_player_index, facing);
    first_person_camera_for_unit_and_vector(unit_index, facing, result);

    field_of_view = player_control_get_field_of_view(controls->local_player_index);
    result->field_of_view = field_of_view;
    if ( camera->last_field_of_view != field_of_view )
    {
        result->___u4.__s0.field_of_view_flags = (1u << _observer_time_valid_bit);
        camera->last_field_of_view = field_of_view;
        result->field_of_view_timer = 0.18000001f;
    }
    if ( g_LastCameraUpdateProc != first_person_camera_update )
        result->field_of_view_timer = 0.0f;

    /* hard snap: zero all interpolation timers, set orientation/position/valid flags */
    result->orientation_timer = 0.0f;
    result->position_timer = 0.0f;
    result->timer = 0.0f;
    result->___u4.__s0.orientation_flags |= ((1u << _observer_time_valid_bit) | (1u << _observer_time_force_bit));
    result->___u4.__s0.position_flags |= ((1u << _observer_time_valid_bit) | (1u << _observer_time_force_bit));
    result->flags |= 1;
}
