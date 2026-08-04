#ifndef SHADER_TRANSPARENT_CHICAGO_MAP_FLAGS_H
#define SHADER_TRANSPARENT_CHICAGO_MAP_FLAGS_H

/* shader_transparent_chicago_map_flags - bit indices for the shader_transparent_chicago_map.flags word.
 * DB-verified: types_enum_values _E2B561EB9739A8C214E5BD6008B31E09 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_transparent_chicago_map_flags {
	_shader_transparent_chicago_map_point_sampled_bit = 0,
	_shader_transparent_chicago_map_alpha_replicate_bit = 1,
	_shader_transparent_chicago_map_u_clamped_bit = 2,
	_shader_transparent_chicago_map_v_clamped_bit = 3,
	NUMBER_OF_SHADER_TRANSPARENT_CHICAGO_MAP_FLAGS = 4,
};

#endif /* SHADER_TRANSPARENT_CHICAGO_MAP_FLAGS_H */

// shader_transparent_chicago_map_flags: DB-verified via types_enum_values _E2B561EB9739A8C214E5BD6008B31E09
