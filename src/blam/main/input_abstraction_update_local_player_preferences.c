/* input_abstraction_update_local_player_preferences @0x83724AE0 — copy a controller's 36-byte control
 * preference entry into the input abstraction layer's per-controller settings table. */

#include <stdint.h>
#include "headers/game_input_preferences.h"
#include "headers/input_abstraction_globals.h"

void input_abstraction_update_local_player_preferences(int16_t controller_index, const game_input_preferences *preferences)
{
    /* recovered: the compiler emitted a pre-incremented 9-dword copy anchored one element back at
     * .invert_look (offset 0x20); each anchor+4*9 lands exactly on the next 36-byte element, so the
     * loop is a whole-struct copy of preferences[0] into player_control_settings[controller_index]. */
    input_abstraction_globals.player_control_settings[controller_index] = *preferences;
}
