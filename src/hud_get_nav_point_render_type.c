/* hud_get_nav_point_render_type @0x837ED634 — decide how a HUD nav-point marker should be drawn based on
 * whether the player has a clear line of sight to its world position. Casts a collision ray from the
 * local player's head toward the nav point (ignoring the player's own unit). Returns 0 (visible) when
 * nothing blocks the ray, or when the only thing hit is the nav point's own reference object; returns 2
 * (occluded) when the view is blocked by anything else. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_result_type.h"
#include "headers/collision_test_flags.h"
#include "headers/waypoint_type.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

int hud_get_nav_point_render_type(int16_t local_player_index, const real_point3d *head,
                                  const real_point3d *position, int reference_object_index)
{
    int unit_index;
    if (local_player_get_player_index(local_player_index) == -1)
        unit_index = -1;
    else
        unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                     local_player_get_player_index(local_player_index))->unit_index;

    real_vector3d direction;
    direction.n[0] = position->n[0] - head->n[0];
    direction.n[1] = position->n[1] - head->n[1];
    direction.n[2] = position->n[2] - head->n[2];

    collision_result collision;
    if (!collision_test_vector(_collision_test_for_line_of_sight_flags, head, &direction, unit_index, &collision)
        || (collision.type == collision_result_object && collision.object_index == reference_object_index))
    {
        return _waypoint_on_screen;
    }
    return _waypoint_occluded;
}
