#ifndef DAMAGE_RESISTANCE_FLAGS_H
#define DAMAGE_RESISTANCE_FLAGS_H

/* damage_resistance_flags — damage_resistance flags.
 * DB-verified: types_enum_values _3EBAA73DDDF0A3F4F69A15401B0E93DC carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum damage_resistance_flags {
	_damage_resistance_takes_shield_damage_for_children_bit = 0,
	_damage_resistance_takes_body_damage_for_children_bit = 1,
	_damage_resistance_always_shields_friendly_damage_bit = 2,
	_damage_resistance_children_take_area_damage_bit = 3,
	_damage_resistance_parent_never_takes_body_damage_for_us_bit = 4,
	_damage_resistance_only_hurt_by_explosives_bit = 5,
	_damage_resistance_only_hurt_while_occupied_bit = 6,
	NUMBER_OF_DAMAGE_RESISTANCE_FLAGS = 7,
};

#endif /* DAMAGE_RESISTANCE_FLAGS_H */

// damage_resistance_flags: DB-verified via types_enum_values _3EBAA73DDDF0A3F4F69A15401B0E93DC
