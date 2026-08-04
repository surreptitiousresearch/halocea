#include "headers/data_array.h"
#include "headers/game_engine.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

int game_engine_player_get_team_index(int player_index)
{
    if (game_engine->game_engine_player_get_team_index)
        return 1;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    return player->local_player_index % 2;
}
