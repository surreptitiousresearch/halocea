#ifndef PLAYER_CONTROL_ACTION_TEST_FLAGS_H
#define PLAYER_CONTROL_ACTION_TEST_FLAGS_H

/* player_control_action_test_flags - bit indices for the player_control_action_test.flags word.
 * DB-verified: types_enum_values _877617AA8533A3657B34B6D80C95EB08 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum player_control_action_test_flags {
	_player_control_action_test_action_bit = 0,
	_player_control_action_test_jump_bit = 1,
	_player_control_action_test_accept_bit = 2,
	_player_control_action_test_back_bit = 3,
	_player_control_action_test_primary_trigger_bit = 4,
	_player_control_action_test_grenade_trigger_bit = 5,
	_player_control_action_test_zoom_bit = 6,
	_player_control_action_test_look_positive_pitch_bit = 7,
	_player_control_action_test_look_negative_pitch_bit = 8,
	_player_control_action_test_look_positive_yaw_bit = 9,
	_player_control_action_test_look_negative_yaw_bit = 10,
	_player_control_action_test_move_forward_bit = 11,
	_player_control_action_test_move_backward_bit = 12,
	_player_control_action_test_move_right_bit = 13,
	_player_control_action_test_move_left_bit = 14,
	NUMBER_OF_PLAYER_CONTROL_ACTION_TEST_FLAGS = 15,
};

#endif /* PLAYER_CONTROL_ACTION_TEST_FLAGS_H */

// player_control_action_test_flags: DB-verified via types_enum_values _877617AA8533A3657B34B6D80C95EB08
