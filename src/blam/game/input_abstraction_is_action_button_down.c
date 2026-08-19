/* input_abstraction_is_action_button_down @0x83724E38 — return the current pressed state of an abstracted
 * action `button` for a given local player, from the input abstraction layer's per-player input state. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

uint8_t input_abstraction_is_action_button_down(int16_t local_player_index, int16_t button)
{
    return input_abstraction_globals.input_state[local_player_index].buttons[button];
}
