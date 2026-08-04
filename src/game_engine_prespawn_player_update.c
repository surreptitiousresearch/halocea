/* game_engine_prespawn_player_update @0x83747704 — give the active game engine a chance to update a
 * player just before respawn. Falls back, when the engine provides no hook, to recording the player's
 * respawn parity (player->team_index = player->local_player_index % 2).
 *
 * DEVIATION: the decompiler's fallback expands the signed modulo-2 as
 * x - 2*((x>>1) + (x<0 && (x&1))); that is exactly x % 2. */

#include "headers/game_engine.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void game_engine_prespawn_player_update(int player_index)
{
    if ( game_engine )
    {
        if ( game_engine->prespawn_player_update )
        {
            game_engine->prespawn_player_update(player_index);
        }
        else
        {
            player_datum *player_record = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
            player_record->team_index = player_record->local_player_index % 2;
        }
    }
}
