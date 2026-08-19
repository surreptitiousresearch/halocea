/* player0_look_pitch_is_inverted @0x836992C8 — return whether local player 0's profile has invert-look
 * enabled. */

#include <stdint.h>
#include "headers/player_ui_profile_view.h"

uint8_t player0_look_pitch_is_inverted(void)
{
    return player_ui_globals.player_data[0].profile.controller_settings.invert_look;
}
