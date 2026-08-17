/* game_engine_render_nav_points @0x8374E310 */
/* game_engine_render_nav_points 0x8374E310 — for each netgame goal that matches the local player,
 * determine its nav-point render type and submit it. Only runs when the goal_radar variant option
 * is on (== 1). The goal array is traversed via a data-segment pointer walk that stops at
 * &global_variant (global_goal[] is laid out immediately before global_variant in the BSS). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/netgame_goal.h"
#include "headers/real_point3d.h"
#include "headers/game_engine.h"
#include "headers/game_engine_type.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern unsigned int game_engine_get_type(void);
extern uint8_t goal_matches_player(player_datum *player, int player_index, int goal_index);
extern int16_t hud_get_nav_point_render_type(int16_t local_player_index, const real_point3d *head, const real_point3d *position, int reference_object_index);
extern void custom_render_nav_point(int16_t local_player_index, const real_point3d *position_pointer, int16_t nav_index, int16_t waypoint_type, uint8_t render_distance);

void game_engine_render_nav_points(int16_t local_player_index)
{
    if ( !game_engine )
        return;

    if ( global_variant.universal_variant.goal_radar != 1 || local_player_index == -1 )
        return;

    int player_index = local_player_get_player_index(local_player_index);
    if ( player_index == -1 )
        return;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    if ( player->unit_index == -1 )
        return;

    real_point3d head;
    unit_get_head_position(player->unit_index, &head);

    int goal_index = 0;
    for ( netgame_goal *goal = global_goal;
          (char *)goal < (char *)&global_variant;
          ++goal, ++goal_index )
    {
        if ( !goal_matches_player(player, player_index, goal_index) )
            continue;

        int16_t nav_point_render_type;
        uint8_t render_distance;

        if ( game_engine_get_type() != game_engine_ctf
             || global_variant.game_engine_variant.ctf.assault
             || goal->team_index == player->team_index
             || goal->team_index == -1 )
        {
            nav_point_render_type = hud_get_nav_point_render_type(local_player_index, &head,
                                                                   &goal->position, -1);
            render_distance = 1;
        }
        else
        {
            nav_point_render_type = hud_get_nav_point_render_type(local_player_index, &head,
                                                                   &goal->position, -1);
            if ( !nav_point_render_type )
                continue;
            render_distance = 0;
        }

        custom_render_nav_point(local_player_index, &goal->position, goal->nav_index,
                                nav_point_render_type, render_distance);
    }
}
