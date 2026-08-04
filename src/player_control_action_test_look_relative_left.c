/* player_control_action_test_look_relative_left @0x836DF130 — return the latched "look left" action-test bit (bit 9 of the player control globals'
 * action_test_flags). */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

uint8_t player_control_action_test_look_relative_left(void)
{
    return (player_control_globals->action_test_flags >> _player_control_action_test_look_positive_yaw_bit) & 1;
}
