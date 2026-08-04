#ifndef CONTROL_BUTTON_H
#define CONTROL_BUTTON_H

/* control_button — abstract control-button index used by input_abstraction_is_action_button_down()
 * and related input tests. DB-verified: types_enum_values _BF398C4F04FEEA76AB52A9BA113F24C0 carries
 * these values verbatim. */
enum control_button {
	FIRST_CONTROL_BUTTON = 0,
	FIRST_ACTION_CONTROL_BUTTON = 0,
	_button_jump = 0,
	_button_switch_grenade = 1,
	_button_action_reload = 2,
	_button_switch_weapon = 3,
	_button_melee_attack = 4,
	_button_flashlight = 5,
	_button_throw_grenade = 6,
	_button_fire = 7,
	_button_start = 8,
	NUMBER_OF_MOVEMENT_CONTROL_BUTTONS = 8,
	_button_back = 9,
	_button_crouch = 10,
	_button_scope_zoom = 11,
	_button_showscores = 12,
	NUMBER_OF_ACTION_CONTROL_BUTTONS = 13,
	FIRST_MOVEMENT_CONTROL_BUTTON = 13,
	_button_move_forward = 13,
	_button_move_backward = 14,
	_button_move_left = 15,
	_button_move_right = 16,
	_button_look_up = 17,
	_button_look_down = 18,
	_button_look_left = 19,
	_button_look_right = 20,
	NUMBER_OF_TOTAL_CONTROL_BUTTONS = 21,
	_button_none = 0x7FFF,
};

#endif /* CONTROL_BUTTON_H */
