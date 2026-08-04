/* scripted_camera_update @0x83705140 — produce this frame's observer command for the scripted (cutscene)
 * camera from camera_script_globals. Four modes:
 *   0 — scripted point: place focus at a world point (or relative to an object), with an optional look-at
 *       offset computed by rotating (point - dot*forward) into the camera's yaw frame.
 *   1 — animated: sample the camera animation's root matrix at the current frame for forward/up/position.
 *   2 — first person: defer to first_person_camera_fake on the relative unit.
 *   3 — dead camera: (re)initialise and update the dead-camera follower.
 * Always decrements the script timer (clamped at 0) and sets the paused flag (0x20) at the end.
 *
 * The DB prototype types `camera` as scripted_camera, but the body and the dead_camera_* callees use
 * dead_camera (the two are the same object); declared as dead_camera.
 *
 * DEVIATIONS (verified against disassembly):
 *   - atan2's second argument is result->___u1.__s0.forward.n[0]; the decompiler mis-split the packed double and left it
 *     as an uninitialized `v12`. The angle is atan2(forward.n[1], forward.n[0]).
 *   - The trailing `__asm fsel f0, f12, f12, f31` is max(timer - elapsed*speed, 0) (f31 == 0.0f).
 *   - frame index and frame_count int<->float conversions are fcfid/fctiwz artifacts, restored as casts.
 *   - speed is a double (game_time_get_speed) used as float in the single-precision expressions. */

#include <stdint.h>
#include "headers/observer_command.h"
#include "headers/object_datum.h"
#include "headers/camera_script_globals.h"
#include "headers/dead_camera.h"
#include "headers/camera_control.h"
#include "headers/animation.h"
#include "headers/model.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/global_tag_instances.h"
#include "headers/camera_script_mode.h"
#include "headers/object_type.h"
#include "headers/observer_command_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/model.h"
extern float game_time_get_speed(void);
extern uint8_t game_time_get_paused(void);
extern void animation_get_root_matrix(const model *model, const animation *animation, int16_t frame_index, real_matrix4x3 *matrix);
extern void first_person_camera_fake(int unit_index, observer_command *result);
extern void dead_camera_new(dead_camera *camera, int16_t local_player_index, int unit_index);
extern void dead_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern double atan2(double y, double x);
extern double cos(double x);
extern double sin(double x);

void scripted_camera_update(dead_camera *camera, const camera_control *controls, observer_command *result)
{
    char have_focus_origin = 1;
    float focus_x = global_origin3d->n[0];
    float focus_y = global_origin3d->n[1];
    float focus_z = global_origin3d->n[2];

    double speed = game_time_get_speed();
    result->flags = (1 << _observer_command_force_time_bit);
    unsigned __int8 paused = game_time_get_paused();
    if (paused)
        result->flags |= (1 << _observer_command_freeze_camera_bit);
    else
        result->flags &= ~(1 << _observer_command_freeze_camera_bit);

    if ((unsigned int)camera_script_globals.mode <= _camera_script_mode_dead)
    {
        if (camera_script_globals.mode == _camera_script_mode_animation)
        {
            /* animated camera: sample the root matrix at the current frame */
            int *animation_graph = TAG_GET(int, camera_script_globals.animation_graph_index);
            const animation *camera_animation =
                (const animation *)(animation_graph[30] + 180 * camera_script_globals.animation_index);
            __int16 frame_count = camera_animation->frame_count;
            __int16 frame = (__int16)(int)((float)frame_count - camera_script_globals.timer * 30.0f);
            if (frame >= 0)
            {
                if (frame > frame_count - 1)
                    frame = frame_count - 1;
            }
            else
                frame = 0;

            real_matrix4x3 root_matrix;
            animation_get_root_matrix(nullptr, camera_animation, frame, &root_matrix);
            int flags = result->flags;
            result->___u1.__s0.focus_distance = 0.0f;
            result->timer = 0.0f;
            result->___u1.__s0.field_of_view = 1.2217305f;
            result->___u1.__s0.forward.n[0] = root_matrix.___u1.n[0][0];
            result->___u1.__s0.forward.n[1] = root_matrix.___u1.n[0][1];
            result->___u1.__s0.forward.n[2] = root_matrix.___u1.n[0][2];
            result->___u1.__s0.up.n[0] = root_matrix.___u1.n[2][0];
            result->___u1.__s0.up.n[1] = root_matrix.___u1.n[2][1];
            result->___u1.__s0.up.n[2] = root_matrix.___u1.n[2][2];
            result->flags = flags | (1 << _observer_command_valid_bit);
            result->___u1.__s0.focus_position.n[0] = root_matrix.___u1.n[3][0];
            result->___u1.__s0.focus_position.n[1] = root_matrix.___u1.n[3][1];
            result->___u1.__s0.focus_position.n[2] = root_matrix.___u1.n[3][2];
        }
        else if (camera_script_globals.mode == _camera_script_mode_first_person)
        {
            if (object_try_and_get_and_verify_type(camera_script_globals.relative_object_index, object_mask_unit))
                first_person_camera_fake(camera_script_globals.relative_object_index, result);
        }
        else if (camera_script_globals.mode)
        {
            /* mode 3: dead camera */
            if (object_try_and_get_and_verify_type(camera_script_globals.relative_object_index, object_mask_unit))
            {
                if (camera_script_globals.first_update)
                    dead_camera_new(camera, controls->local_player_index,
                                    camera_script_globals.relative_object_index);
                dead_camera_update(camera, controls, result);
            }
        }
        else
        {
            /* mode 0: scripted point */
            if (camera_script_globals.relative_object_index != -1)
            {
                object_datum *object = object_try_and_get_and_verify_type(
                    camera_script_globals.relative_object_index, object_mask_all);
                if (object)
                {
                    focus_x = object->object.bounding_sphere_center.n[0];   /* +160 */
                    focus_y = object->object.bounding_sphere_center.n[1];
                    focus_z = object->object.bounding_sphere_center.n[2];
                }
                else
                    have_focus_origin = 0;
            }

            if (have_focus_origin)
            {
                result->timer = speed == 0.0 ? 0.0f : (camera_script_globals.timer / (float)speed);
                result->___u1.__s0.field_of_view = camera_script_globals.field_of_view;
                result->___u1.__s0.forward = camera_script_globals.forward;
                result->___u1.__s0.up = camera_script_globals.up;

                if (camera_script_globals.relative_object_index == -1)
                {
                    result->___u1.__s0.focus_position.n[0] = camera_script_globals.point.n[0];
                    result->___u1.__s0.focus_position.n[1] = camera_script_globals.point.n[1];
                    result->___u1.__s0.focus_position.n[2] = camera_script_globals.point.n[2];
                    result->flags |= (1 << _observer_command_valid_bit);
                }
                else
                {
                    float angle = (float)atan2(result->___u1.__s0.forward.n[1], result->___u1.__s0.forward.n[0]);

                    /* project the script point onto the view direction (clamped behind the camera) */
                    float dot = result->___u1.__s0.forward.n[0] * camera_script_globals.point.n[0]
                                + (result->___u1.__s0.forward.n[2] * camera_script_globals.point.n[2]
                                   + result->___u1.__s0.forward.n[1] * camera_script_globals.point.n[1]);
                    if (dot > 0.0f)
                        dot = 0.0f;

                    result->___u1.__s0.focus_position.n[0] = focus_x;
                    result->___u1.__s0.focus_position.n[1] = focus_y;
                    result->___u1.__s0.focus_position.n[2] = focus_z;
                    result->___u1.__s0.focus_distance = -dot;

                    float offset_x = camera_script_globals.point.n[0] - result->___u1.__s0.forward.n[0] * dot;
                    float offset_y = camera_script_globals.point.n[1] - result->___u1.__s0.forward.n[1] * dot;
                    float offset_z = camera_script_globals.point.n[2] - result->___u1.__s0.forward.n[2] * dot;

                    float sine = (float)sin((double)angle);
                    float cosine = (float)cos((double)angle);
                    result->___u1.__s0.focus_offset.n[0] = sine * offset_y + cosine * offset_x;
                    result->___u1.__s0.focus_offset.n[2] = offset_z;
                    result->___u1.__s0.focus_offset.n[1] = sine * offset_x - cosine * offset_y;
                    result->___u4.__s0.position_flags = 1;
                    result->___u5.__s0.position_timer = 0.0f;
                    result->flags |= (1 << _observer_command_valid_bit);
                }
            }
        }
    }

    /* decrement the script timer, clamped at 0 */
    float new_timer = camera_script_globals.timer - controls->seconds_elapsed * (float)speed;
    camera_script_globals.first_update = 0;
    camera_script_globals.timer = new_timer >= 0.0f ? new_timer : 0.0f;
}
