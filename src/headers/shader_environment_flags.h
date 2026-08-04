#ifndef SHADER_ENVIRONMENT_FLAGS_H
#define SHADER_ENVIRONMENT_FLAGS_H

/* shader_environment_flags - bit indices for the shader_environment.flags word.
 * DB-verified: types_enum_values _D2D05304CB2B4913B5C72DB668C68CF4 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_environment_flags {
	_shader_environment_alpha_tested_bit = 0,
	_shader_environment_bump_map_is_specular_mask_bit = 1,
	_shader_environment_true_atmospheric_fog_bit = 2,
	NUMBER_OF_SHADER_ENVIRONMENT_FLAGS = 3,
};

#endif /* SHADER_ENVIRONMENT_FLAGS_H */

// shader_environment_flags: DB-verified via types_enum_values _D2D05304CB2B4913B5C72DB668C68CF4
