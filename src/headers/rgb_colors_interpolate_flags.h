#ifndef RGB_COLORS_INTERPOLATE_FLAGS_H
#define RGB_COLORS_INTERPOLATE_FLAGS_H

/* rgb_colors_interpolate_flags - bit indices for the rgb_colors_interpolate.flags word.
 * DB-verified: types_enum_values _AF3DB77DD7FC2AA521EABF150544DB92 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum rgb_colors_interpolate_flags {
	_rgb_colors_interpolate_in_hsv_space_bit = 0,
	_rgb_colors_interpolate_along_farthest_hue_path_bit = 1,
	NUMBER_OF_RGB_COLORS_INTERPOLATE_FLAGS = 2,
};

#endif /* RGB_COLORS_INTERPOLATE_FLAGS_H */

// rgb_colors_interpolate_flags: DB-verified via types_enum_values _AF3DB77DD7FC2AA521EABF150544DB92
