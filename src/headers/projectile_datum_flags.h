#ifndef PROJECTILE_DATUM_FLAGS_H
#define PROJECTILE_DATUM_FLAGS_H

/* projectile_datum_flags - bit indices for the projectile_datum.flags word.
 * DB-verified: types_enum_values _89EC88A6B6D5DEC08ED36EC7CDCCAA4F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum projectile_datum_flags {
	_projectile_has_nonzero_angular_velocity_bit = 0,
	_projectile_tracer_bit = 1,
	_projectile_collided_once_bit = 2,
	_projectile_attached_bit = 3,
	_projectile_stopped_after_collision_bit = 4,
	_projectile_counting_down_bit = 5,
	_projectile_already_super_exploded_bit = 6,
	_projectile_will_super_explode_bit = 7,
	NUMBER_OF_PROJECTILE_DATUM_FLAGS = 8,
};

#endif /* PROJECTILE_DATUM_FLAGS_H */

// projectile_datum_flags: DB-verified via types_enum_values _89EC88A6B6D5DEC08ED36EC7CDCCAA4F
