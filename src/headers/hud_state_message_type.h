#pragma once

/* hud_state_message_type — the message_index argument of hud_set_state_message (@0x836A46E0): an
 * ordinal into the hud_globals state-message tag block naming which persistent HUD prompt is shown.
 * hud_show_action_response (@0x83707FB0) selects all thirteen from the player's action_result and
 * respawn-failure code, and every one of its literals lands on the same-named member (exit_vehicle
 * -> 7, swap_for_powerup -> 1, swap_for_weapon -> 4, respawn_failure_moving -> 9,
 * respawn_failure_combat -> 11, ...). Members keep their DB spelling (_hud_message_*); the header
 * is named for the family's own terminator because src/headers/hud_message.h is the message-slot
 * STRUCT. Source: types_enum_values $72ED394C1C18F2B8D9BADE33748E6301 (DB-verified). */
enum hud_state_message_type {
	_hud_message_pickup = 0,
	_hud_message_swap_powerup = 1,
	_hud_message_touch_device = 2,
	_hud_message_custom_device = 3,
	_hud_message_swap_weapon = 4,
	_hud_message_remind_to_switch_weapons = 5,
	_hud_message_enter_vehicle = 6,
	_hud_message_exit_vehicle = 7,
	_hud_message_flip_vehicle = 8,
	_hud_message_respawn_failed_moving = 9,
	_hud_message_respawn_failed_unsafe = 10,
	_hud_message_respawn_failed_combat = 11,
	_hud_message_respawn_failed_vehicle = 12,
	NUMBER_OF_HUD_STATE_MESSAGE_TYPES = 13,
};

// hud_state_message_type: DB-verified via types_enum_values $72ED394C1C18F2B8D9BADE33748E6301
