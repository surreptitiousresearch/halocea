/* game_engine_state_message @0x83748CD0 — records a game-engine status message (e.g. "you are dead",
 * "you are the last man standing") to be displayed for a player's HUD. */

#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


void game_engine_state_message(int player_index, int message, int message_data)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    player->multiplayer.state_message = message;
    player->multiplayer.state_message_data = message_data;
}
