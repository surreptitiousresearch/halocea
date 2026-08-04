/* player_control_action_test_look_relative_all_directions @0x836DF110 — return true only if all four
 * relative-look direction bits (mask 0x780 of action_test_flags) are set. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

/* mask 0x780 = the four relative-look direction bits */
#define PLAYER_CONTROL_ACTION_TEST_LOOK_MASK ( \
      (1u << _player_control_action_test_look_positive_pitch_bit) \
    | (1u << _player_control_action_test_look_negative_pitch_bit) \
    | (1u << _player_control_action_test_look_positive_yaw_bit) \
    | (1u << _player_control_action_test_look_negative_yaw_bit))

uint8_t player_control_action_test_look_relative_all_directions(void)
{
    return (~player_control_globals->action_test_flags & PLAYER_CONTROL_ACTION_TEST_LOOK_MASK) == 0;
}
