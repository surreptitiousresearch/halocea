#ifndef BUTTON_PRESET_FLAGS_H
#define BUTTON_PRESET_FLAGS_H

/* button_preset_flags — button_preset flags.
 * DB-verified: types_enum_values _38D114EEC33FD4854D1F2061B90F46F0 carries these named
 * values verbatim. Bit-index members use as (1u << _bit); *_MASK / *_MAXIMUM
 * / large members are composite masks or constants, used as-is. */
enum button_preset_flags {
	_button_preset_standard = 0,
	_button_preset_southpaw = 1,
	_button_preset_jumpy = 2,
	_button_preset_bumperjumper = 3,
	_button_preset_boxer = 4,
	_button_preset_greenthumb = 5,
	_button_preset_theduke = 6,
	_button_preset_hero = 7,
	_button_preset_custom = 8,
	NUMBER_OF_BUTTON_PRESETS = 9,
	_joystick_preset_standard = 0,
	_joystick_preset_south_paw = 1,
	_joystick_preset_legacy = 2,
	_joystick_preset_legacy_south_paw = 3,
	NUMBER_OF_JOYSTICK_PRESETS = 4,
	NUMBER_OF_DEFAULT_PROFILES = 2,
	_profile_is_default_bit = 0,
	_profile_is_builtin_bit = 1,
	_profile_finished_game_bit = 2,
	NUMBER_OF_PROFILE_FLAGS = 3,
	_default_profile_index_mask = 65280,
	_look_sensitivity_lowest = 1,
	_look_sensitivity_highest = 10,
};

#endif /* BUTTON_PRESET_FLAGS_H */

// button_preset_flags: DB-verified via types_enum_values _38D114EEC33FD4854D1F2061B90F46F0
