#ifndef PLAYER_ACTION_RESULT_H
#define PLAYER_ACTION_RESULT_H

/* player_action_result — the "action" prompt/result a player can be offered for a nearby object,
 * stored/posted via player_set_action_result.
 * DB-verified: types_enum_values _8590DCD59B573681AFABE3E9EBAC9E83 carries these named values verbatim. */
enum player_action_result {
	_player_action_result_reload                 = 0,
	_player_action_result_pickup_powerup         = 1,
	_player_action_result_pickup_weapon          = 2,
	_player_action_result_exit_vehicle           = 3,
	_player_action_result_swap_for_grenades      = 4,
	_player_action_result_swap_for_powerup       = 5,
	_player_action_result_swap_for_weapon        = 6,
	_player_action_result_add_weapon_to_inventory = 7,
	_player_action_result_enter_vehicle          = 8,
	_player_action_result_evict_from_vehicle     = 9,
	_player_action_result_touch_device           = 10,
	_player_action_result_flip_vehicle           = 11,
	NUMBER_OF_PLAYER_ACTION_RESULTS              = 12,
};

#endif /* PLAYER_ACTION_RESULT_H */
