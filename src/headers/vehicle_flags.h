#ifndef VEHICLE_FLAGS_H
#define VEHICLE_FLAGS_H

/* vehicle_flags - bit indices for the vehicle.flags word.
 * DB-verified: types_enum_values _D53385B9F43CC716F4A4ECD7235A00F0 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum vehicle_flags {
	_vehicle_blurred_bit = 0,
	_vehicle_hovering_bit = 1,
	_vehicle_control_crouch_bit = 2,
	_vehicle_control_jump_bit = 3,
	_vehicle_upending_bit = 4,
	NUMBER_OF_VEHICLE_FLAGS = 5,
};

#endif /* VEHICLE_FLAGS_H */

// vehicle_flags: DB-verified via types_enum_values _D53385B9F43CC716F4A4ECD7235A00F0
