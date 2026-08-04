#ifndef PLAYER_CONTROL_FLAGS_H
#define PLAYER_CONTROL_FLAGS_H

/* player_control_flags - bit indices for the player_control.flags word.
 * DB-verified: types_enum_values _AF48D60F8FC4B9D5DB7766EAE4234074 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum player_control_flags {
	_player_control_rotate_weapons_bit = 0,
	_player_control_rotate_grenades_bit = 1,
	_player_control_zoom_bit = 2,
	_player_control_debug_rotate_units_bit = 3,
	_player_control_debug_rotate_all_units_bit = 4,
	_player_control_debug_ninja_rope_bit = 5,
	NUMBER_OF_PLAYER_CONTROL_FLAGS = 6,
};

#endif /* PLAYER_CONTROL_FLAGS_H */

// player_control_flags: DB-verified via types_enum_values _AF48D60F8FC4B9D5DB7766EAE4234074
