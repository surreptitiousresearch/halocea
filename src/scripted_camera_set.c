/* scripted_camera_set @0x83704F68 — point the scripted camera at a scenario cutscene camera point (104-byte
 * records: position +0x28, euler angles +0x34, field of view +0x40, defaulting to 70 degrees / 1.2217305 rad
 * when zero). The camera holds the point for tick_count/30 seconds, optionally anchored to an object, then
 * the director and observer are updated to apply it immediately. */

#include <stdint.h>
#include "headers/camera_script_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_camera_point.h"
#include "headers/real_point3d.h"
#include "headers/real_euler_angles3d.h"
#include "headers/camera_script_mode.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
#include "headers/real_euler_angles3d.h"
extern void vectors3d_from_euler_angles3d(real_vector3d *forward, real_vector3d *up, const real_euler_angles3d *angles);
extern void director_update(float dt);
extern void observer_update(float dt, uint8_t predict_local_player_movement);

void scripted_camera_set(int16_t camera_point_index, int16_t tick_count, int relative_to_object_index)
{
    scenario_cutscene_camera_point *camera_point =
        (scenario_cutscene_camera_point *)global_scenario->cutscene_camera_points.address + camera_point_index;
    camera_script_globals.camera_point_index = camera_point_index;
    camera_script_globals.mode = _camera_script_mode_point;
    camera_script_globals.first_update = 1;
    camera_script_globals.point = camera_point->position;
    vectors3d_from_euler_angles3d(&camera_script_globals.forward, &camera_script_globals.up,
                                  &camera_point->orientation);
    float field_of_view = camera_point->field_of_view;
    if ( field_of_view == 0.0f )
        field_of_view = 1.2217305f;
    camera_script_globals.field_of_view = field_of_view;
    camera_script_globals.relative_object_index = relative_to_object_index;
    camera_script_globals.timer = (float)(tick_count / 30);
    director_update(0.0f);
    observer_update(0.000099999997f, 0);
}
