/* player_control_inhibit_buttons @ 0x836DE4F0 — block a set of button bits for a local player.
 * When uninhibit_when_released is set, the bits are also flagged to auto-clear once the player
 * lets go of them. */

#include <stdint.h>
#include "headers/player_control_globals.h"

void player_control_inhibit_buttons(int16_t local_player_index, uint16_t button_flags,
                                    uint8_t uninhibit_when_released)
{
    player_control *player = &player_control_globals->players[local_player_index];

    player->inhibited_button_bit_vector |= button_flags;
    if ( uninhibit_when_released )
        player->reset_button_when_released_bit_vector |= button_flags;
}
