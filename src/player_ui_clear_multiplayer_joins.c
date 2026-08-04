/* player_ui_clear_multiplayer_joins @0x83698978 — clear the pending "join next multiplayer game"
 * state for both local player slots.
 *
 * DEVIATION: the decompiler walks a raw -4 bias base off &player_ui_globals by the 1988-byte
 * player_data stride; that resolves to player_data[i].prejoined_multiplayer (@1984). */

#include "headers/player_ui_globals.h"

void player_ui_clear_multiplayer_joins(void)
{
    for ( int local_player_index = 0; local_player_index < 2; ++local_player_index )
    {
        player_ui_globals.player_data[local_player_index].prejoined_multiplayer = 0;
        player_ui_globals.join_players_to_next_multiplayer_game[local_player_index] = 0;
    }
}
