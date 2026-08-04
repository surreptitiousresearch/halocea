/* game_engine_hud_draw_messages @0x83749A60 — true if the game engine's HUD messages should NOT be drawn
 * for a player: no game engine running, no valid player, or that player's score-alpha faded out. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/blam_data_globals.h"


uint8_t game_engine_hud_draw_messages(int player_index)
{
    if ( game_engine && player_index != -1 )
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

        if ( player->local_player_index != -1 )
            return game_engine_globals.score_alpha[player->local_player_index] <= 0.0f;
    }
    return 1;
}
