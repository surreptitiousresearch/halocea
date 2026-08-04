#ifndef SHADER_TRANSPARENT_GENERIC_FLAGS_H
#define SHADER_TRANSPARENT_GENERIC_FLAGS_H

/* shader_transparent_generic_flags — shader_transparent_generic flags.
 * DB-verified: types_enum_values _6F94FE51DE8617FB0B57E1DDFF8750AD carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum shader_transparent_generic_flags {
	_shader_transparent_generic_alpha_tested_bit = 0,
	_shader_transparent_generic_decal_bit = 1,
	_shader_transparent_generic_two_sided_bit = 2,
	_shader_transparent_generic_first_map_is_in_screenspace_bit = 3,
	_shader_transparent_generic_draw_before_water_bit = 4,
	_shader_transparent_generic_ignore_effect_bit = 5,
	_shader_transparent_generic_scale_first_map_with_distance_bit = 6,
	_shader_transparent_generic_numeric_bit = 7,
	NUMBER_OF_SHADER_TRANSPARENT_GENERIC_FLAGS = 8,
};

#endif /* SHADER_TRANSPARENT_GENERIC_FLAGS_H */

// shader_transparent_generic_flags: DB-verified via types_enum_values _6F94FE51DE8617FB0B57E1DDFF8750AD
