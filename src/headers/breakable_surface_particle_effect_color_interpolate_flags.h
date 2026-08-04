#ifndef BREAKABLE_SURFACE_PARTICLE_EFFECT_COLOR_INTERPOLATE_FLAGS_H
#define BREAKABLE_SURFACE_PARTICLE_EFFECT_COLOR_INTERPOLATE_FLAGS_H

/* breakable_surface_particle_effect_color_interpolate_flags - bit indices for the breakable_surface_particle_effect_color_interpolate.flags word.
 * DB-verified: types_enum_values _ACEEECA7ED38BAC011BC1248EDFF2E7D carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum breakable_surface_particle_effect_color_interpolate_flags {
	_breakable_surface_particle_effect_color_interpolate_in_hsv_bit = 0,
	_breakable_surface_particle_effect_color_interpolate_along_farthest_hue_path_bit = 1,
	NUMBER_OF_BREAKABLE_SURFACE_PARTICLE_EFFECT_FLAGS = 2,
};

#endif /* BREAKABLE_SURFACE_PARTICLE_EFFECT_COLOR_INTERPOLATE_FLAGS_H */

// breakable_surface_particle_effect_color_interpolate_flags: DB-verified via types_enum_values _ACEEECA7ED38BAC011BC1248EDFF2E7D
