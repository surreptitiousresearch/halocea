/* director_rotate_cameras @ 0x836E4E88 — advance a local player's camera to the next entry in its
 * allowed-camera list (wrapping) and install the matching camera proc and freshly-initialized per-camera
 * state. Camera kind codes are director_camera_mode: _camera_following (third person),
 * _camera_orbiting, _camera_flying (free), _camera_editor (no change — leave current proc),
 * _camera_first_person. After a real change the camera-change pause and debug input scale are reset,
 * debug controls disabled, and the new mode name is echoed to the console. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/director_camera_mode.h"
#include "headers/flying_camera.h"
#include "headers/orbiting_camera.h"
#include "headers/following_camera.h"
#include "headers/first_person_camera.h"
#include "headers/observer_command.h"
#include "headers/camera_control.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
extern void orbiting_camera_new(orbiting_camera *camera, float distance, const real_vector3d *facing);
extern void orbiting_camera_update(orbiting_camera *camera, const camera_control *controls, observer_command *result);
extern void flying_camera_new_from_point_and_vector(flying_camera *camera, const real_point3d *focus, const real_vector3d *orientation);
extern void flying_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result);
extern void first_person_camera_new(first_person_camera *camera);
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);
extern void following_camera_new(following_camera *camera);
extern void following_camera_update(following_camera *camera, const camera_control *controls, observer_command *result);
extern void console_printf(uint8_t clear, const char *format, ...);

extern void director_set_camera(int16_t local_player_index, void (*camera_proc)(void *camera_data, const camera_control *, observer_command *), uint8_t interpolate);
void director_rotate_cameras(int16_t local_player_index, const int16_t *cameras, int16_t camera_count)
{
    director *dir = &director_globals.local_players[local_player_index];
    int next = dir->camera_mode_index + 1;
    int wrapped = (int16_t)(next - next / camera_count * camera_count);
    unsigned int kind;
    /* camera_proc is one generic slot in the original binary; each per-kind update fn takes its
     * concrete camera type, so the function-pointer casts below are binary-faithful type-puns. */
    void (*proc)(flying_camera *, const camera_control *, observer_command *) = 0;
    int changed = 1;

    dir->camera_mode_index = (int16_t)wrapped;
    kind = (uint16_t)cameras[wrapped];

    if ( kind <= _camera_first_person )
    {
        if ( kind == _camera_orbiting )
        {
            orbiting_camera_new((orbiting_camera *)dir->camera_data, dir->command.___u1.__s0.focus_distance,
                                &dir->command.___u1.__s0.forward);  /* third arg is &command.forward (disasm: r31+0x7C) */
            proc = (void (*)(flying_camera *, const camera_control *, observer_command *))orbiting_camera_update;
        }
        else if ( kind == _camera_flying )
        {
            flying_camera_new_from_point_and_vector((flying_camera *)dir->camera_data,
                                                    &dir->command.___u1.__s0.focus_position, &dir->command.___u1.__s0.forward);
            proc = flying_camera_update;
        }
        else if ( kind != _camera_editor )
        {
            if ( cameras[wrapped] )   /* kind == _camera_first_person */
            {
                first_person_camera_new((first_person_camera *)dir->camera_data);
                proc = (void (*)(flying_camera *, const camera_control *, observer_command *))first_person_camera_update;
            }
            else                      /* kind == _camera_following */
            {
                following_camera_new((following_camera *)dir->camera_data);
                proc = (void (*)(flying_camera *, const camera_control *, observer_command *))following_camera_update;
            }
        }
        else
        {
            changed = 0;   /* _camera_editor: keep current proc */
        }
    }
    else
    {
        changed = 0;
    }

    if ( changed )
    {
        /* DEVIATION: director_set_camera@0x836E48E8 inlined here (zero-xref donor); interpolate folded to 1 (camera_change_pause=1.0 unconditional) — collapsed to a call. */
        director_set_camera(local_player_index, (void (*)(void *, const camera_control *, observer_command *))proc, 1);
    }

    console_printf(0, "%s camera", director_camera_mode_names[cameras[dir->camera_mode_index]]);
}
