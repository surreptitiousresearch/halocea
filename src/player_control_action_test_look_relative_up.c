/* player_control_action_test_look_relative_up @0x836DF160 — return the latched "look up" action-test bit (bit 7 of the player control globals'
 * action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_look_relative_up(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_look_positive_pitch_bit) & 1;
}
