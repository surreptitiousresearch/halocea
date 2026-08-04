/* following_camera_update @ 0x837C2A80 — the camera proc for the third-person "following" view. It
 * focuses on the controlled unit's origin, accumulates the player's look deltas into a facing offset
 * (decaying it to zero when no input is active), clamps pitch to +/-90 deg, and places the eye behind
 * the unit at a distance derived from the seat's camera track (camera_track_splut), scaled by the
 * camera's distance_scale and pushed out further when crouched/zoomed. The focus velocity is taken from
 * the unit so the camera leads moving targets, and the up vector is derived from the forward vector. */

#include <stdint.h>
#include "headers/following_camera.h"
#include "headers/unit_camera.h"
#include "headers/unit_camera_info.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"


extern void player_control_get_unit_camera_info(int16_t local_player_index, unit_camera_info *camera_info);
extern const real_euler_angles2d *player_control_get_facing_angles(int16_t local_player_index);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void camera_track_splut(unit_camera *unit_camera, float pitch, real_vector3d *out_track);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern void observer_up_from_forward(const real_vector3d *forward, real_vector3d *up);
extern double cos(double x);
extern double sin(double x);
extern float  __fsqrts(float x);

/* object data pointer for an object handle (object header datum: data ptr at int element 3*idx+2) */
static int object_data_ptr(int object_handle)
{
    return ((int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum);
}

void following_camera_update(following_camera *camera, const camera_control *controls, observer_command *result)
{
    unit_camera_info info;
    __int16 seat_index;
    int unit_index;

    player_control_get_unit_camera_info(controls->local_player_index, &info);
    seat_index = info.seat_index;
    unit_index = info.unit_index;

    result->flags = 0;
    result->___u1.__s0.focus_position.n[0] = info.unit_origin.n[0];
    result->___u1.__s0.focus_position.n[1] = info.unit_origin.n[1];
    result->___u1.__s0.focus_position.n[2] = info.unit_origin.n[2];
    result->___u1.__s0.field_of_view = 1.2217305f;
    result->timer = 0.0f;

    /* re-target snap when the followed unit/seat changes */
    if ( camera->initialized && (unit_index != camera->unit_index || seat_index != camera->seat_index) )
        result->timer = 1.0f;
    camera->unit_index = unit_index;
    camera->seat_index = seat_index;

    if ( info.unit_camera )
    {
        /* +520 = unit_datum.unit (_unit_datum @500) .control_flags @+20 */
        int flags = *(int *)(object_data_ptr(unit_index) + 520);
        unsigned char crouched = ((flags & (1u << _unit_control_crouch_modifier_bit)) != 0
                               || (flags & (1u << _unit_control_jump_bit)) != 0) ? 1 : 0;
        const real_euler_angles2d *facing_angles;
        real_euler_angles2d look;
        real_vector3d track;
        double pitch, sum_pitch;
        double cos_pitch, sin_pitch;
        double pulled_distance, distance;

        /* crouch/zoom transition kicks a half-second focus-offset interpolation */
        if ( crouched != camera->crouched )
        {
            result->___u4.__s0.focus_offset_flags = 1;
            if ( result->___u5.__s0.focus_offset_timer < 0.5f )
                result->___u5.__s0.focus_offset_timer = 0.5f;
            camera->crouched = crouched;
        }

        if ( controls->active )
        {
            camera->facing_offset.n[0] = controls->facing_delta.n[0] + camera->facing_offset.n[0];
            camera->facing_offset.n[1] = controls->facing_delta.n[1] + camera->facing_offset.n[1];
            result->___u4.__s0.orientation_flags = 1;
            if ( result->___u5.__s0.orientation_timer < 0.40000001f )
                result->___u5.__s0.orientation_timer = 0.40000001f;
        }
        else if ( camera->facing_offset.n[0] != 0.0f || camera->facing_offset.n[1] != 0.0f )
        {
            camera->facing_offset.n[0] = 0.0f;
            camera->facing_offset.n[1] = 0.0f;
        }

        facing_angles = player_control_get_facing_angles(controls->local_player_index);
        sum_pitch = (camera->facing_offset.n[1] + facing_angles->n[1]);
        /* yaw = player facing yaw + accumulated yaw offset (decompiler misrendered the yaw load) */
        look.n[0] = (facing_angles->n[0] + camera->facing_offset.n[0]);
        look.n[1] = facing_angles->n[1];
        if ( sum_pitch >= -1.5707964 )
            look.n[1] = sum_pitch <= 1.5707964 ? (float)sum_pitch : 1.5707964f;
        else
            look.n[1] = -1.5707964f;

        vector3d_from_euler_angles2d(&result->___u1.__s0.forward, &look);

        pitch = look.n[1];
        camera_track_splut(info.unit_camera, look.n[1], &track);

        result->___u1.__s0.focus_distance = __fsqrts(((track.n[0] * track.n[0])
                                       + ((track.n[2] * track.n[2])
                                               + (track.n[1] * track.n[1]))));
        cos_pitch = cos(pitch);
        result->___u1.__s0.focus_offset.n[0] =
            (((result->___u1.__s0.focus_distance * (float)cos_pitch) + track.n[0]) * camera->distance_scale);
        result->___u1.__s0.focus_offset.n[1] = -(camera->distance_scale * track.n[1]);
        sin_pitch = sin(pitch);
        result->___u1.__s0.focus_offset.n[2] =
            ((result->___u1.__s0.focus_distance * (float)sin_pitch) + track.n[2]) * camera->distance_scale;

        /* clamp the eye-back distance to at least 0.6 */
        pulled_distance = (result->___u1.__s0.focus_distance - 0.60000002f);
        distance = 0.60000002f;
        if ( (((float)pulled_distance * camera->distance_scale) + 0.60000002f) > 0.60000002f )
            distance = (((float)pulled_distance * camera->distance_scale) + 0.60000002f);
        result->___u1.__s0.focus_distance = distance;

        object_get_velocities(info.unit_index, &result->focus_velocity, 0);
        result->flags |= 1u;
    }

    observer_up_from_forward(&result->___u1.__s0.forward, &result->___u1.__s0.up);
    camera->initialized = 1;
}
