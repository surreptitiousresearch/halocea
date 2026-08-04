#ifndef DEVICE_POSITION_FLAGS_H
#define DEVICE_POSITION_FLAGS_H

/* device_position_flags - bit indices for the device_position.flags word.
 * DB-verified: types_enum_values _DE6EE5BBED96BB7B76A35A939CE693C0 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum device_position_flags {
	_device_position_loops_bit = 0,
	_device_position_animation_not_interpolated_bit = 1,
	NUMBER_OF_DEVICE_DEFINITION_FLAGS = 2,
};

#endif /* DEVICE_POSITION_FLAGS_H */

// device_position_flags: DB-verified via types_enum_values _DE6EE5BBED96BB7B76A35A939CE693C0
