/* editor_camera_new @0x837C3680 — create the free-fly editor camera. The first time, it seeds the focus
 * point from the scenario's first player spawn (position + facing), or the origin if there are none. It then
 * builds a flying camera looking along the focus angles, records it as the active editor camera for the
 * primary local player, and runs the current camera-mode's per-frame translate hook.
 * DEVIATION: the disassembly's else-branch zeroes only the focus position (3 floats); the decompiler's
 * extra `angles = 0` is spurious and omitted. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/flying_camera.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/editor_camera_focus_t.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern void (*translate_funcs[][2])(flying_camera *camera);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void flying_camera_new_from_point_and_vector(flying_camera *camera, const real_point3d *focus, const real_vector3d *orientation);

void editor_camera_new(flying_camera *camera, int16_t local_player_index)
{
    if ( !initialized_0 )
    {
        if ( global_scenario->players.count && global_scenario->players.address )
        {
            const float *spawn = (const float *)global_scenario->players.address;
            editor_camera_focus.position.__s1.x = spawn[0];
            editor_camera_focus.position.__s1.y = spawn[1];
            editor_camera_focus.position.__s1.z = spawn[2];
            editor_camera_focus.angles.__s1.yaw = spawn[3];
        }
        else
        {
            editor_camera_focus.position.__s1.x = 0.0f;
            editor_camera_focus.position.__s1.y = 0.0f;
            editor_camera_focus.position.__s1.z = 0.0f;
        }
    }
    initialized_0 = 1;

    real_vector3d forward;
    vector3d_from_euler_angles2d(&forward, &editor_camera_focus.angles);
    flying_camera_new_from_point_and_vector(camera, &editor_camera_focus.position, &forward);
    if ( !local_player_index )
        editor_camera = camera;
    if ( camera_mode )
        translate_funcs[camera_mode][1](camera);
}
