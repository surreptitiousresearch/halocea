/* should_flip_sticks_for_local_player @0x83731C40 — true when the local player's joystick control
 * scheme requires left/right stick icons to be swapped ("Southpaw"-style layouts 1 and 3). */

#include <stdint.h>
#include "headers/game_input_preferences.h"
#include "headers/button_preset_flags.h"

extern int16_t local_player_get_next(int16_t local_player_index);
extern void input_abstraction_get_local_player_preferences(int16_t local_player_index, game_input_preferences *preferences);

uint8_t should_flip_sticks_for_local_player(int16_t local_player_index)
{
    if (local_player_index == -1)
        local_player_index = local_player_get_next(-1);

    /* FAITHFUL: zeroes the whole struct (four 8-byte stores + one 4-byte store) before the
     * conditional fill-in below, rather than leaving it uninitialized. */
    game_input_preferences preferences = {0};

    if (local_player_index != -1)
        input_abstraction_get_local_player_preferences(local_player_index, &preferences);

    return preferences.joystick_controls == _joystick_preset_south_paw
        || preferences.joystick_controls == _joystick_preset_legacy_south_paw;
}
