#include <stdint.h>
#include "headers/player_ui_globals.h"

/* prejoined_multiplayer (_player_data @1984 == 0x7C0), loaded as a byte (lbzx); callers
 * normalize with clrlwi r3,24 => 8-bit Blam boolean return.
 * DEVIATION: decompiler computed a misleading join_players_to_next_multiplayer_game[] index. */
uint8_t player_ui_local_player_wants_to_play_multiplayer(int16_t local_player_index)
{
    return player_ui_globals.player_data[local_player_index].prejoined_multiplayer;
}
