/* player_control_action_test_look_relative_right @0x836DF148 — return the latched "look right" action-test bit (bit 10 of the player control globals'
 * action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_look_relative_right(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_look_negative_yaw_bit) & 1;
}
