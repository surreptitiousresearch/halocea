#ifndef INPUT_ABSTRACTION_MODE_FLAGS_H
#define INPUT_ABSTRACTION_MODE_FLAGS_H

/* input_abstraction_mode_flags - bit indices for the input_abstraction_mode.flags word.
 * DB-verified: types_enum_values _CDE4294CCDF6E1CB74286E3C375F3D9B carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum input_abstraction_mode_flags {
	_input_abstraction_mode_game_bit = 0,
	_input_abstraction_mode_events_bit = 1,
	_input_abstraction_mode_keyboard_bit = 2,
	_input_abstraction_mode_configuration_bit = 3,
	NUMBER_OF_INPUT_ABSTRACTION_MODE_BITS = 4,
	_input_abstraction_mode_unknown = 255,
};

#endif /* INPUT_ABSTRACTION_MODE_FLAGS_H */

// input_abstraction_mode_flags: DB-verified via types_enum_values _CDE4294CCDF6E1CB74286E3C375F3D9B
