/* game_engine_player_changed_team @0x837492B8 — force-kills the player (with at least a 90-tick respawn
 * delay) then notifies the active game engine's player_changed_team callback, if the current variant
 * defines one. */

#include "headers/data_array.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern void player_force_kill(int player_index, int respawn_time);

void game_engine_player_changed_team(int player_index, int new_team)
{
    if ( game_engine )
    {
        datum_try_and_get(player_data, player_index);

        int respawn_time = global_variant.universal_variant.respawn_time;
        if ( respawn_time <= 90 )
            respawn_time = 90;

        player_force_kill(player_index, respawn_time);

        if ( game_engine->player_changed_team )
            game_engine->player_changed_team(player_index, new_team);
    }
}
