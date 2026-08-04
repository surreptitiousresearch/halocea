#ifndef BUILD_SPRITE_ROTATIONAL_FLAGS_H
#define BUILD_SPRITE_ROTATIONAL_FLAGS_H

/* build_sprite_rotational_flags - bit indices for the build_sprite_rotational.flags word.
 * DB-verified: types_enum_values _7552E30B62B129B1DBA2240BCBF41B39 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum build_sprite_rotational_flags {
	_build_sprite_rotational_viewer_space_bit = 0,
	_build_sprite_rotational_sideways_rotation_animates_bit = 1,
	NUMBER_OF_BUILD_SPRITE_ROTATIONAL_FLAGS = 2,
};

#endif /* BUILD_SPRITE_ROTATIONAL_FLAGS_H */

// build_sprite_rotational_flags: DB-verified via types_enum_values _7552E30B62B129B1DBA2240BCBF41B39
