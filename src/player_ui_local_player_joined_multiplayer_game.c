#include <stdint.h>
#include "headers/player_ui_globals.h"

/* Mark that this local player has joined the current multiplayer game and should
 * be auto-joined into the next one.  DEVIATION: decompiler rendered the two
 * writes as raw indexing off join_players_to_next_multiplayer_game with 1988-byte
 * strides; offsets resolve to player_data[idx].prejoined_multiplayer (0x7C0) and
 * join_players_to_next_multiplayer_game[idx] (0xF88). */
void player_ui_local_player_joined_multiplayer_game(int16_t local_player_index)
{
    player_ui_globals.player_data[local_player_index].prejoined_multiplayer = 1;
    player_ui_globals.join_players_to_next_multiplayer_game[local_player_index] = 1;
}
