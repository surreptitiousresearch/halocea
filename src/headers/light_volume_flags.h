#ifndef LIGHT_VOLUME_FLAGS_H
#define LIGHT_VOLUME_FLAGS_H

/* light_volume_flags — light_volume flags.
 * DB-verified: types_enum_values _6D221C76ECB21AFCB0D2CC451B30A794 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum light_volume_flags {
	_light_volume_color_interpolate_in_hsv_bit = 0,
	_light_volume_color_interpolate_along_farthest_hue_path_bit = 1,
	NUMBER_OF_LIGHT_VOLUME_FLAGS = 2,
};

#endif /* LIGHT_VOLUME_FLAGS_H */

// light_volume_flags: DB-verified via types_enum_values _6D221C76ECB21AFCB0D2CC451B30A794
