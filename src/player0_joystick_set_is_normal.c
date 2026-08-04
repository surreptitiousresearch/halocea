/* player0_joystick_set_is_normal @0x836992D8 — return whether local player 0's joystick layout is one of
 * the "normal" presets (preset 0 or 1), i.e. not an inverted/southpaw-style configuration. */

#include <stdint.h>
#include "headers/player_ui_profile_view.h"
#include "headers/button_preset_flags.h"

uint8_t player0_joystick_set_is_normal(void)
{
    if ( !player_ui_globals.player_data[0].profile.controller_settings.joystick_preset )
        return 1;
    if ( player_ui_globals.player_data[0].profile.controller_settings.joystick_preset == _joystick_preset_south_paw )
        return 1;
    return 0;
}
