#ifndef SHADER_TRANSPARENT_GLASS_FLAGS_H
#define SHADER_TRANSPARENT_GLASS_FLAGS_H

/* shader_transparent_glass_flags - bit indices for the shader_transparent_glass.flags word.
 * DB-verified: types_enum_values _9B7ABCEC95E89AFE07B7A43BDC7CD5C3 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_transparent_glass_flags {
	_shader_transparent_glass_alpha_tested_bit = 0,
	_shader_transparent_glass_decal_bit = 1,
	_shader_transparent_glass_two_sided_bit = 2,
	_shader_transparent_glass_bump_map_is_specular_mask_bit = 3,
	NUMBER_OF_SHADER_TRANSPARENT_GLASS_FLAGS = 4,
};

#endif /* SHADER_TRANSPARENT_GLASS_FLAGS_H */

// shader_transparent_glass_flags: DB-verified via types_enum_values _9B7ABCEC95E89AFE07B7A43BDC7CD5C3
