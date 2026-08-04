#ifndef DAMAGE_DATA_FLAGS_H
#define DAMAGE_DATA_FLAGS_H

/* damage_data_flags — damage_data flags.
 * DB-verified: types_enum_values _5FF1DE2D4DB1B284DDD5FC446B98A8F5 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum damage_data_flags {
	_damage_area_of_effect_bit = 0,
	_damage_create_localized_effect_bit = 1,
	_damage_kill_instantly_bit = 2,
	_damage_from_weapon_bit = 3,
	_damage_silent_bit = 4,
	_damage_bypasses_shields_bit = 5,
	_damage_damaged_one_object_bit = 6,
	_damage_no_statistics_bit = 7,
	NUMBER_OF_DAMAGE_DATA_FLAGS = 8,
};

#endif /* DAMAGE_DATA_FLAGS_H */

// damage_data_flags: DB-verified via types_enum_values _5FF1DE2D4DB1B284DDD5FC446B98A8F5
