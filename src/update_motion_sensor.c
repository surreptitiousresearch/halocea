/* update_motion_sensor @0x837BD3D8 — recompute one local player's motion-sensor (radar) blips for the frame,
 * into the currently-active double-buffer slot (motion_sensor_globals->sensor_active_index). Only runs on the
 * frames flagged for update. For each of the 16 tracked unit slots it fetches the unit's object, and — if the
 * unit should be drawn and lies within motion_sensor_range horizontally of the player's camera — stores a
 * signed-byte blip position (offset/range scaled to +/-127); otherwise it marks the blip type 6 ("gone") and
 * clears the tracked-unit slot. It then records the sensor's world reference point (camera position) and yaw
 * (facing + 90 deg), gathers the game-engine's custom motion-sensor positions (flags/objectives), and appends
 * the in-range ones as compacted custom blips, trimming the active count by however many fell out of range.
 *
 * DEVIATION: the camera reference point store the decompiler rendered as __PAIR64__(0x82000000, v26.n[1]) is a
 * stale-FPR-shadow misread — disasm (0x837BD478/48C) is a plain 8-byte copy of {camera.n[0], camera.n[1]} into
 * reference_point. The decompiler flattened current_unit_indices[i] as sensor_data[0].blips[i+330] and the blip
 * fields as raw v5[4*i+k]; here they use the modeled motion_sensor_datum fields (current_unit_indices @0x528 of
 * the player datum; blips/reference_point/yaw/custom_blips of the active sensor). The 0.0*0.0 z terms in the
 * horizontal distance are bit-exact no-ops but reproduced. Object origin is at float offset 40/41/42 (=+160). */

#include <stdint.h>
#include "headers/motion_sensor_globals.h"   /* full nested layout; NOT motion_sensor_globals_definition.h (stub twin) */
#include "headers/hud_globals.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/blip_type.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t should_draw_object(int object_index);
extern const real_euler_angles2d *player_control_get_facing_angles(int16_t local_player_index);
extern uint8_t game_engine_player_get_custom_motion_sensor_positions(int player_index, real_point2d *points, char *goal_indices, int16_t max_points);


extern void tiny_point2d_set(tiny_point2d *tiny_position, real_point2d *position);
void update_motion_sensor(int16_t local_player_index)
{
    motion_sensor_player_datum *player_sensor = &motion_sensor_globals->motion_sensors[local_player_index];
    game_time_get();   /* result unused — compiled-in timestamp read */

    if ( !motion_sensor_globals->update )
        return;

    __int16 sensor_active_index = motion_sensor_globals->sensor_active_index;

    int unit_index;
    if ( local_player_get_player_index(local_player_index) == -1 )
        unit_index = -1;
    else
        unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                local_player_get_player_index(local_player_index))->unit_index;

    motion_sensor_datum *sensor = &player_sensor->sensor_data[sensor_active_index];
    if ( unit_index == -1 )
        return;

    real_point3d camera_position;
    unit_get_camera_position(unit_index, &camera_position);
    sensor->reference_point.n[0] = camera_position.n[0];
    sensor->reference_point.n[1] = camera_position.n[1];

    for ( int i = 0; i < 16; i = (__int16)(i + 1) )
    {
        int object_index = player_sensor->current_unit_indices[i];
        if ( !object_try_and_get_and_verify_type(object_index, object_mask_unit) )
            continue;

        unsigned __int8 draw = should_draw_object(object_index);
        float center_x = sensor->reference_point.n[0];
        float center_y = sensor->reference_point.n[1];
        object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
        float object_x = object->object.bounding_sphere_center.n[0];
        float object_y = object->object.bounding_sphere_center.n[1];

        float dx = object_x - center_x;
        float dy = object_y - center_y;
        float range = hud_globals->defaults.motion_sensor_range;
        if ( !draw
          || ((dy * dy) + ((0.0f * 0.0f) + (dx * dx)))
                     > (range * range) )
        {
            sensor->blips[i].type = _blip_type_none;
            /* recovered: *(int*)&current_unit_indices[i] -> current_unit_indices[i] (redundant cast on int[16] member) */
            player_sensor->current_unit_indices[i] = -1;
        }
        /* DEVIATION: this is an inlined copy of tiny_point2d_set@0x837BC698 (zero-xref out-of-line twin) -- dx/dy is the caller's constructed real_point2d, folded in place of the pointer arg; range is re-fetched from hud_globals->defaults.motion_sensor_range inside the callee itself, not passed. Collapsed to a call. */
        else
        {
            real_point2d displacement = { dx, dy };
            tiny_point2d_set(&sensor->blips[i].pt, &displacement);
        }
    }

    sensor->yaw = player_control_get_facing_angles(local_player_index)->n[0] + 1.5707964f;

    int player_index = local_player_get_player_index(local_player_index);
    real_point2d custom_positions[16];
    sensor->custom_blips_active = game_engine_player_get_custom_motion_sensor_positions(player_index,
            custom_positions, sensor->custom_blip_goal_indices, 16);

    real_point3d custom_camera;
    unit_get_camera_position(unit_index, &custom_camera);

    int out_of_range = 0;
    if ( sensor->custom_blips_active )
    {
        for ( int i = 0; i < sensor->custom_blips_active; ++i )
        {
            float dx = custom_positions[i].n[0] - custom_camera.n[0];
            float dy = custom_positions[i].n[1] - custom_camera.n[1];
            custom_positions[i].n[0] = dx;
            custom_positions[i].n[1] = dy;
            float range = hud_globals->defaults.motion_sensor_range;
            if ( ((dy * dy) + ((0.0f * 0.0f) + (dx * dx)))
                    > (range * range) )
            {
                ++out_of_range;
            }
            /* DEVIATION: second inlined copy of tiny_point2d_set@0x837BC698 in the same function (custom-blip compaction loop) -- dx/dy (v22/v23) is the local displacement, position arg folded away; range re-fetched from the global inside the callee. Collapsed to a call. */
            else
            {
                real_point2d displacement = { dx, dy };
                tiny_point2d_set(&sensor->custom_blips[i - out_of_range], &displacement);
            }
        }
    }

    sensor->custom_blips_active -= out_of_range;
}
