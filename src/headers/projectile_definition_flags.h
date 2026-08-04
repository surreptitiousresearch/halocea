#ifndef PROJECTILE_DEFINITION_FLAGS_H
#define PROJECTILE_DEFINITION_FLAGS_H

/* projectile_definition_flags - bit indices for the projectile_definition.flags word.
 * DB-verified: types_enum_values _7ADC464C17E3CD20AA4A608953566FA8 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum projectile_definition_flags {
	_projectile_oriented_along_velocity_bit = 0,
	_projectile_aim_ballistic_bit = 1,
	_projectile_detonation_max_time_if_attached_bit = 2,
	_projectile_super_combining_explosion_bit = 3,
	_projectile_combine_initial_velocity_with_parent_velocity_bit = 4,
	_projectile_random_detonation_time_when_attached_bit = 5,
	_projectile_minimum_unattached_detonation_time = 6,
	NUMBER_OF_PROJECTILE_DEFINITION_FLAGS = 7,
};

#endif /* PROJECTILE_DEFINITION_FLAGS_H */

// projectile_definition_flags: DB-verified via types_enum_values _7ADC464C17E3CD20AA4A608953566FA8
