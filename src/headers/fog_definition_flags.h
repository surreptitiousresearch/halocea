#ifndef FOG_DEFINITION_FLAGS_H
#define FOG_DEFINITION_FLAGS_H

/* fog_definition_flags - bit indices for the fog_definition.flags word.
 * DB-verified: types_enum_values _EA877BB24225E94E6E20B1C2216189C4 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum fog_definition_flags {
	_fog_definition_is_water_bit = 0,
	_fog_definition_atmosphere_dominant_bit = 1,
	_fog_definition_screen_effect_only_bit = 2,
	NUMBER_OF_FOG_DEFINITION_FLAGS = 3,
};

#endif /* FOG_DEFINITION_FLAGS_H */

// fog_definition_flags: DB-verified via types_enum_values _EA877BB24225E94E6E20B1C2216189C4
