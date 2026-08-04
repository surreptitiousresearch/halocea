#ifndef LENS_FLARE_REFLECTION_FLAGS_H
#define LENS_FLARE_REFLECTION_FLAGS_H

/* lens_flare_reflection_flags - bit indices for the lens_flare_reflection.flags word.
 * DB-verified: types_enum_values _EDE7FD90D28161147AF686734A85FB70 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum lens_flare_reflection_flags {
	_lens_flare_reflection_rotate_from_center_of_screen_bit = 0,
	_lens_flare_reflection_radius_not_scaled_by_distance_bit = 1,
	_lens_flare_reflection_radius_scaled_by_occlusion_bit = 2,
	_lens_flare_reflection_zbuffer_bit = 3,
	NUMBER_OF_LENS_FLARE_REFLECTION_FLAGS = 4,
};

#endif /* LENS_FLARE_REFLECTION_FLAGS_H */

// lens_flare_reflection_flags: DB-verified via types_enum_values _EDE7FD90D28161147AF686734A85FB70
