#ifndef SCENARIO_WEAPON_FLAGS_H
#define SCENARIO_WEAPON_FLAGS_H

/* scenario_weapon_flags - bit indices for the scenario_weapon.flags word.
 * DB-verified: types_enum_values _C1F4D0F8625E858870815D54D47D04B4 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum scenario_weapon_flags {
	_weapon_created_at_rest_bit = 0,
	_weapon_obsolete_bit = 1,
	_weapon_does_accelerate_bit = 2,
	NUMBER_OF_SCENARIO_WEAPON_FLAGS = 3,
};

#endif /* SCENARIO_WEAPON_FLAGS_H */

// scenario_weapon_flags: DB-verified via types_enum_values _C1F4D0F8625E858870815D54D47D04B4
