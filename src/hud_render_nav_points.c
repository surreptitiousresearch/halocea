/* hud_render_nav_points 0x837EDE8C — draw the local player's HUD navigation waypoints. For each of the four
 * nav-point slots, resolve a world position from the slot's type (top nibble of the packing word): type 0 from a
 * scenario cutscene flag, type 1 from a referenced object's origin, anything else from the game engine's goal
 * position. Deactivated/invalid slots (nav_index 0xFFFF, no reference, or type nibble 0xF) are marked invalid by
 * forcing the top nibble to 0xF. Each resolved point, raised by the slot's z_offset, is submitted to
 * custom_render_nav_point with the slot's screen_type nibble; finally the game engine renders its own nav points.
 *
 * Faithful note: local_player_get_player_index is genuinely called twice (no CSE in the original). */

#include <stdint.h>
#include "headers/hud_globals.h"
#include "headers/hud_nav_point_datum.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/object_datum.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/hud_nav_point_type.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *game_engine_get_goal_position(real_point3d *result, int16_t index);
extern void custom_render_nav_point(int16_t local_player_index, const real_point3d *position_pointer, int16_t nav_index, int16_t waypoint_type, uint8_t render_distance);
extern void game_engine_render_nav_points(int16_t local_player_index);

void hud_render_nav_points(int16_t local_player_index)
{
    if ( local_player_index != -1
         && local_player_get_player_index(local_player_index) != -1
         && DATA_ARRAY_ELEMENT(player_data, player_datum,
                     local_player_get_player_index(local_player_index))->unit_index != -1
         && hud_globals->waypoint.arrow_bitmap.index != -1 )
    {
        hud_nav_point_player_datum *player_nav = &nav_point_data[local_player_index];
        for ( int slot = 0; slot < MAXIMUM_ACTIVE_NAV_POINTS; slot = (int16_t)(slot + 1) )
        {
            hud_nav_point_datum *nav = &player_nav->nav_points[slot];
            int reference_index;
            int type;
            if ( (uint16_t)nav->nav_index == 0xFFFF
                 || (reference_index = nav->reference_index) == -1
                 /* recovered: (int)((unsigned)nav->packing << 16) >> 28 -> nav->type (signed 4-bit) */
                 || (type = nav->type) == -1 )
            {
                /* recovered: nav->packing |= 0xF000 -> nav->type = -1 (deactivated nibble 0xF) */
                nav->type = -1;
                continue;
            }

            real_point3d position;
            if ( type == _nav_point_flag )
            {
                /* cutscene flag reference */
                scenario_cutscene_flag *cutscene_flag =
                    &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[reference_index];
                position = cutscene_flag->position;
            }
            else if ( type == _nav_point_object )
            {
                /* object reference: skip the slot if the object no longer exists */
                if ( !object_try_and_get_and_verify_type(reference_index, object_mask_all) )
                    continue;
                /* object datum base from the object-header table (element[3*index+2]); +160 (0xA0) is
                 * object_datum.object.bounding_sphere_center (_object_datum @0x9C) */
                const object_datum *object_data =
                        ((const object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, nav->reference_index)->datum);
                position = object_data->object.bounding_sphere_center;
            }
            else
            {
                real_point3d goal_buffer[8];
                position = *game_engine_get_goal_position(goal_buffer, reference_index);
            }

            position.n[2] = nav->z_offset + position.n[2];
            /* recovered: (__int16)(16 * nav->packing) >> 12 -> nav->screen_type (signed 4-bit) */
            custom_render_nav_point(local_player_index, &position, nav->nav_index,
                                    nav->screen_type, 1u);
        }
    }
    game_engine_render_nav_points(local_player_index);
}
