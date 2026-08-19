/* game_engine_player_get_custom_motion_sensor_positions @0x8374E230 — collect the 2D positions (and goal
 * indices) of every netgame goal relevant to a player, for the motion sensor, when the variant's goal
 * radar option is off. Returns the number of points written (capped at max_points).
 *
 * Note: the compiled loop terminates by comparing the goal pointer against &global_variant — the global
 * that happens to sit at the end of the global_goal array — rather than an explicit count; reproduced
 * verbatim. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/netgame_goal.h"
#include "headers/player_datum.h"
#include "headers/real_point2d.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern uint8_t goal_matches_player(player_datum *player, int player_index, int goal_index);

uint8_t game_engine_player_get_custom_motion_sensor_positions(int player_index, real_point2d *points, char *goal_indices, int16_t max_points)
{
    int point_count = 0;

    if ( game_engine && !global_variant.universal_variant.goal_radar && player_index != -1 )
    {
        int goal_index = 0;
        netgame_goal *goal = global_goal;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        do
        {
            if ( goal_matches_player(player, player_index, goal_index)
              && (int16_t)point_count < max_points )
            {
                goal_indices[(int16_t)point_count] = goal_index;
                points[(int16_t)point_count].n[0] = goal->position.n[0];
                points[(int16_t)point_count].n[1] = goal->position.n[1];
                point_count = (int16_t)(point_count + 1);
            }
            ++goal;
            ++goal_index;
        }
        while ( (char *)goal < (char *)&global_variant );   /* array-end sentinel, as compiled */
    }
    return point_count;
}
