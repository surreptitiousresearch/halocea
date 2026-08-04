/* player_ui_look_pitch_is_inverted @0x83699308 — return the invert-look controller setting for a
 * local player's saved profile (0 when no local player is bound).
 *
 * DEVIATION: the decompiler renders the load as an offset into the byte array
 * join_players_to_next_multiplayer_game[1988 * i - 3675]; that absolute address resolves to
 * player_data[i].profile.controller_settings.invert_look (player_data stride 1988), which is the
 * named field used here. */

#include <stdint.h>
#include "headers/player_ui_globals.h"

uint8_t player_ui_look_pitch_is_inverted(int16_t local_player_index)
{
    if ( local_player_index == -1 )
        return 0;
    return player_ui_globals.player_data[local_player_index].profile.controller_settings.invert_look;
}
