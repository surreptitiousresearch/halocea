#include <stdint.h>
#include "headers/player_ui_globals.h"

/* DEVIATION: decompiler gave confusing join_players index; disasm shows lhzx at player_data[lpi]+0x128
 * (== player_data[lpi].profile.last_single_player_map_played @296). */
int16_t player_ui_get_last_single_player_level_played(int16_t local_player_index)
{
    return player_ui_globals.player_data[local_player_index].profile.last_single_player_map_played;
}
