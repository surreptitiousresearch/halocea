#ifndef TERRAIN_EFFECT_FLAGS_H
#define TERRAIN_EFFECT_FLAGS_H

/* terrain_effect_flags - bit indices for the terrain_effect.flags word.
 * DB-verified: types_enum_values _E74A562A4D0B19DCDF8D5514A7ED50D0 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum terrain_effect_flags {
	_terrain_effect_first_type_bit = 0,
	_terrain_effect_last_type_bit = 15,
	_terrain_effect_light_bit = 16,
	_terrain_effect_heavy_bit = 17,
	NUMBER_OF_TERRAIN_EFFECT_FLAGS = 18,
};

#endif /* TERRAIN_EFFECT_FLAGS_H */

// terrain_effect_flags: DB-verified via types_enum_values _E74A562A4D0B19DCDF8D5514A7ED50D0
