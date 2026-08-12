#pragma once

/* input_constants — the keyboard/input capacity constants family: code-table sizes, the
 * buffered-keystroke ring capacity, key-repeat timing (ms), and the analog button
 * thresholds. Named for input_update_keyboard's ASCII table walk (enum drain Wave E).
 * Source: types_enum_values $B87160B14F46976CF6A20C8C069F92CE (DB-verified, members
 * verbatim in DB order; duplicate value 64 is the DB's own). */
enum input_constants {
	NUMBER_OF_VIRTUAL_CODES = 256,
	NUMBER_OF_ASCII_CODES = 128,
	MAXIMUM_BUFFERED_KEYSTROKES = 64,
	KEY_REPEAT_DELAY = 500,
	KEY_REPEAT_RATE = 100,
	ANALOG_BUTTON_DOWN = 64,
	ANALOG_BUTTON_UP = 32,
};

// input_constants: DB-verified via types_enum_values $B87160B14F46976CF6A20C8C069F92CE
