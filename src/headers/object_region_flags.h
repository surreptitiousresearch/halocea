#ifndef OBJECT_REGION_FLAGS_H
#define OBJECT_REGION_FLAGS_H

/* object_region_flags - bit indices for the object_region.flags word.
 * DB-verified: types_enum_values _A47C362C7716CFBF9E6526F9CDC1DCF2 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum object_region_flags {
	_object_region_lives_until_object_dies_bit = 0,
	_object_region_forces_object_to_die_bit = 1,
	_object_region_dies_when_object_dies_bit = 2,
	_object_region_dies_when_object_is_damaged_bit = 3,
	_object_region_missing_when_shield_is_zero_bit = 4,
	_object_region_inhibits_melee_attack_bit = 5,
	_object_region_inhibits_ranged_attack_bit = 6,
	_object_region_inhibits_walking_bit = 7,
	_object_region_forces_drop_weapon_bit = 8,
	_object_region_head_destroyed_scream_bit = 9,
	NUMBER_OF_DAMAGE_REGION_FLAGS = 10,
};

#endif /* OBJECT_REGION_FLAGS_H */

// object_region_flags: DB-verified via types_enum_values _A47C362C7716CFBF9E6526F9CDC1DCF2
