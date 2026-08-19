/* director_move_fly_camera @0x836E4C98 — switch local player 0's director camera into free-fly mode,
 * seeded at the given focus point looking along the given direction. Builds a flying camera from the
 * point and vector, disables debug controls, installs flying_camera_update as the camera proc, and sets
 * the free-fly camera mode.
 *
 * ABI: three 12-byte structs are passed by value — position (r3:r4/arg_10), forward (r5:r6/arg_20), and
 * up (r7:r8). The disassembly homes only r3-r6 and passes &position and &forward to
 * flying_camera_new_from_point_and_vector by address; the third struct `up` is passed by the caller but
 * never read here. Declared with all three params (2026-07-31) to match the caller and DB prototype. */

#include "headers/director.h"
#include "headers/director_camera_mode.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/flying_camera.h"
#include "headers/camera_control.h"
#include "headers/observer_command.h"

extern void flying_camera_new_from_point_and_vector(flying_camera *camera, const real_point3d *focus, const real_vector3d *orientation);
extern void flying_camera_update(flying_camera *camera, const camera_control *controls, observer_command *result);

void director_move_fly_camera(real_point3d position, real_vector3d forward, real_vector3d up)
{
    (void)up;   /* passed by the caller (r7:r8) but not read by this function */
    flying_camera_new_from_point_and_vector(
        (flying_camera *)director_globals.local_players[0].camera_data, &position, &forward);
    director_globals.local_players[0].debug_controls = 0;
    director_globals.local_players[0].camera_proc = flying_camera_update;
    director_globals.local_players[0].debug_input_scale = 1.0;
    director_globals.local_players[0].camera_mode_index = _camera_flying;
}
