/* player_control_action_test_look_relative_down @0x836DF178 — return the latched "look down" action-test bit (bit 8 of the player control globals'
 * action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_look_relative_down(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_look_negative_pitch_bit) & 1;
}
