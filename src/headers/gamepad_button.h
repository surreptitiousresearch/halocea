#pragma once

/* gamepad_button — Xbox gamepad button indices as delivered in event_record.value.button_value.button
 * (analog buttons 0-7, then binary buttons 8-15).
 * Source: reference enum _F8C0FA1066F6E2019BAA777490D9D204 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum gamepad_button {
	FIRST_GAMEPAD_ANALOG_BUTTON = 0,
	_gamepad_analog_button_a = 0,
	_gamepad_analog_button_b = 1,
	_gamepad_analog_button_x = 2,
	_gamepad_analog_button_y = 3,
	_gamepad_analog_button_black = 4,
	_gamepad_analog_button_white = 5,
	_gamepad_analog_button_left_trigger = 6,
	_gamepad_analog_button_right_trigger = 7,
	NUMBER_OF_GAMEPAD_ANALOG_BUTTONS = 8,
	FIRST_GAMEPAD_BINARY_BUTTON = 8,
	_gamepad_binary_button_dpad_up = 8,
	_gamepad_binary_button_dpad_down = 9,
	_gamepad_binary_button_dpad_left = 10,
	_gamepad_binary_button_dpad_right = 11,
	_gamepad_binary_button_start = 12,
	_gamepad_binary_button_back = 13,
	_gamepad_binary_button_left_thumb = 14,
	_gamepad_binary_button_right_thumb = 15,
	NUMBER_OF_GAMEPAD_BUTTONS = 16,
	NUMBER_OF_GAMEPAD_BINARY_BUTTONS = 8,
};

// gamepad_button: DB-verified via types_enum_values _F8C0FA1066F6E2019BAA777490D9D204 (anonymous compiled enum, names sans leading _)
