/* scripted_camera_set_camera_point_relative @0x83705040 — put the scripted camera at an explicit
 * object-relative viewpoint: scripted-point mode with no camera point, the given frame and FOV (0 means
 * the 70° default, 1.2217305 rad), the relative object latched, and the timer set to tick_count / 30
 * seconds (integer division, as compiled). The director and observer are stepped once (zero / epsilon dt)
 * so the new viewpoint takes effect immediately.
 *
 * DEVIATION (bug classes 1+9): fov (f1) burns the r6 slot, which the decompiler fused with r7 into an
 * __int64 pun; observer_update's second argument is the stale r4 left by director_update — reproduced as
 * an unspecified interpolate flag of 0 (the observed register value is indeterminate; disasm shows no
 * assignment, and the callee treats nonzero as interpolate). */

#include <stdint.h>
#include "headers/camera_script_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/camera_script_mode.h"
#include "headers/blam_data_globals.h"


extern void director_update(float dt);
extern void observer_update(float dt, uint8_t predict_local_player_movement);

void scripted_camera_set_camera_point_relative(const real_point3d *position, const real_vector3d *forward, const real_vector3d *up, float fov, int16_t tick_count, int relative_to_object_index)
{
    camera_script_globals.mode = _camera_script_mode_point;
    camera_script_globals.camera_point_index = -1;
    camera_script_globals.point = *position;
    camera_script_globals.forward = *forward;
    camera_script_globals.up = *up;
    if ( fov == 0.0f )
        fov = 1.2217305f;
    camera_script_globals.relative_object_index = relative_to_object_index;
    camera_script_globals.field_of_view = fov;
    camera_script_globals.timer = (float)(tick_count / 30);

    director_update(0.0f);
    observer_update(0.000099999997f, 0);
}
