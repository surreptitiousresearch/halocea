#ifndef DAMAGE_EFFECT_DEFINITION_FLAGS_H
#define DAMAGE_EFFECT_DEFINITION_FLAGS_H

/* damage_effect_definition_flags — damage_effect_definition flags.
 * DB-verified: types_enum_values _2E7F1CC72043A642DF4F87747CAB6F82 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum damage_effect_definition_flags {
	_damage_effect_dont_scale_damage_by_distance_bit = 0,
	NUMBER_OF_DAMAGE_EFFECT_DEFINITION_FLAGS = 1,
};

#endif /* DAMAGE_EFFECT_DEFINITION_FLAGS_H */

// damage_effect_definition_flags: DB-verified via types_enum_values _2E7F1CC72043A642DF4F87747CAB6F82
