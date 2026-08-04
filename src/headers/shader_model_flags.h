#ifndef SHADER_MODEL_FLAGS_H
#define SHADER_MODEL_FLAGS_H

/* shader_model_flags - bit indices for the shader_model.flags word.
 * DB-verified: types_enum_values _D0ABFC1C1AEEB75EEB9933FA4DFD5ECB carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_model_flags {
	_shader_model_detail_after_reflection_bit = 0,
	_shader_model_two_sided_bit = 1,
	_shader_model_not_alpha_tested_bit = 2,
	_shader_model_alpha_blended_decal_bit = 3,
	_shader_model_true_atmospheric_fog_bit = 4,
	_shader_model_nocull_two_sided_bit = 5,
	NUMBER_OF_SHADER_MODEL_FLAGS = 6,
};

#endif /* SHADER_MODEL_FLAGS_H */

// shader_model_flags: DB-verified via types_enum_values _D0ABFC1C1AEEB75EEB9933FA4DFD5ECB
