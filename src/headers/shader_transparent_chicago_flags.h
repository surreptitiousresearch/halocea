#ifndef SHADER_TRANSPARENT_CHICAGO_FLAGS_H
#define SHADER_TRANSPARENT_CHICAGO_FLAGS_H

/* shader_transparent_chicago_flags - bit indices for the shader_transparent_chicago.flags word.
 * DB-verified: types_enum_values _9C3E72C470762B979DC922557B182A5F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_transparent_chicago_flags {
	_shader_transparent_chicago_alpha_tested_bit = 0,
	_shader_transparent_chicago_decal_bit = 1,
	_shader_transparent_chicago_two_sided_bit = 2,
	_shader_transparent_chicago_first_map_is_in_screenspace_bit = 3,
	_shader_transparent_chicago_draw_before_water_bit = 4,
	_shader_transparent_chicago_ignore_effect_bit = 5,
	_shader_transparent_chicago_scale_first_map_with_distance_bit = 6,
	_shader_transparent_chicago_numeric_bit = 7,
	NUMBER_OF_SHADER_TRANSPARENT_CHICAGO_FLAGS = 8,
};

#endif /* SHADER_TRANSPARENT_CHICAGO_FLAGS_H */

// shader_transparent_chicago_flags: DB-verified via types_enum_values _9C3E72C470762B979DC922557B182A5F
