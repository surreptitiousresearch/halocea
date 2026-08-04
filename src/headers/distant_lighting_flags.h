#ifndef DISTANT_LIGHTING_FLAGS_H
#define DISTANT_LIGHTING_FLAGS_H

/* distant_lighting_flags - bit indices for the distant_lighting.flags word.
 * DB-verified: types_enum_values _FB98AF4DB60F9F4736B5CFC018521D8E carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum distant_lighting_flags {
	_distant_lighting_raycast_sideways_bit = 0,
	_distant_lighting_block_on_textures_bit = 1,
	_distant_lighting_brighten_bit = 2,
	NUMBER_OF_STATIC_LIGHTING_FLAGS = 3,
};

#endif /* DISTANT_LIGHTING_FLAGS_H */

// distant_lighting_flags: DB-verified via types_enum_values _FB98AF4DB60F9F4736B5CFC018521D8E
