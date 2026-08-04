/* player_inside_hill @0x8382BFF8 — King of the Hill test: true when the player's unit position (object
 * floats +160/+164/+168) lies within the hill's vertical band [hill_bottom, hill_top] and inside the
 * hill's 2D convex hull. False for an invalid player or a player with no unit (player+52 == NONE). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/king_globals.h"
#include "headers/real_point2d.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t convex_hull2d_test_point(int16_t count, const real_point2d *points, const real_point2d *point, float epsilon);

unsigned __int8 player_inside_hill(int player_index)
{
    if ( player_index == -1 )
        return 0;

    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    if ( unit_index == -1 )
        return 0;

    object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    float unit_z = unit->object.bounding_sphere_center.n[2];
    if ( unit_z < king_globals.hill_bottom || unit_z > king_globals.hill_top )
        return 0;

    real_point2d unit_position;
    unit_position.n[0] = unit->object.bounding_sphere_center.n[0];
    unit_position.n[1] = unit->object.bounding_sphere_center.n[1];
    return convex_hull2d_test_point(king_globals.hill_point_count, king_globals.convex_hull,
                                    &unit_position, 0.0f);
}
