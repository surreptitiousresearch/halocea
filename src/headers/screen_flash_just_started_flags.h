#ifndef SCREEN_FLASH_JUST_STARTED_FLAGS_H
#define SCREEN_FLASH_JUST_STARTED_FLAGS_H

/* screen_flash_just_started_flags - bit indices for the screen_flash_just_started.flags word.
 * DB-verified: types_enum_values _8D0427A62874DB9EDE6DF6E18C6F29CD carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum screen_flash_just_started_flags {
	_screen_flash_just_started_bit = 0,
	_camera_impulse_just_started_bit = 1,
	_camera_shake_just_started_bit = 2,
};

#endif /* SCREEN_FLASH_JUST_STARTED_FLAGS_H */

// screen_flash_just_started_flags: DB-verified via types_enum_values _8D0427A62874DB9EDE6DF6E18C6F29CD
