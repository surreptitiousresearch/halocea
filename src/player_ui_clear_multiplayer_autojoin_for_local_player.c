/* player_ui_clear_multiplayer_autojoin_for_local_player @0x83698928 */
#include <stdint.h>
#include "headers/player_ui_globals.h"

/* prejoined_multiplayer (_player_data @1984); same field as player_ui_local_player_wants_to_play_multiplayer */
void player_ui_clear_multiplayer_autojoin_for_local_player(int16_t local_player_index)
{
    player_ui_globals.player_data[local_player_index].prejoined_multiplayer = 0;
}
