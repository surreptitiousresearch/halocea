/* player_control_action_test_move_relative_all_directions @0x836DF0F0 — return true only if all four
 * relative-move direction bits (mask 0x7800 of action_test_flags) are set. */

#include <stdint.h>
#include "headers/player_control_globals.h"
#include "headers/player_control_action_test_flags.h"

/* mask 0x7800 = the four relative-move direction bits */
#define PLAYER_CONTROL_ACTION_TEST_MOVE_MASK ( \
      (1u << _player_control_action_test_move_forward_bit) \
    | (1u << _player_control_action_test_move_backward_bit) \
    | (1u << _player_control_action_test_move_right_bit) \
    | (1u << _player_control_action_test_move_left_bit))

uint8_t player_control_action_test_move_relative_all_directions(void)
{
    return (~player_control_globals->action_test_flags & PLAYER_CONTROL_ACTION_TEST_MOVE_MASK) == 0;
}
