#ifndef PLAYER_CONTROL_CAMERA_OFF_FLAGS_H
#define PLAYER_CONTROL_CAMERA_OFF_FLAGS_H

/* player_control_camera_off_flags - bit indices for the player_control_camera_off.flags word.
 * DB-verified: types_enum_values _8640091B4631A3B62A7C027F4EA5A927 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum player_control_camera_off_flags {
	_player_control_camera_off_bit = 0,
	NUMBER_OF_GLOBAL_PLAYER_CONTROL_FLAGS = 1,
};

#endif /* PLAYER_CONTROL_CAMERA_OFF_FLAGS_H */

// player_control_camera_off_flags: DB-verified via types_enum_values _8640091B4631A3B62A7C027F4EA5A927
