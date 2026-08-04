#ifndef SHADER_EFFECT_FLAGS_H
#define SHADER_EFFECT_FLAGS_H

/* shader_effect_flags — bit indices for the shader_effect flags word.
 * DB-verified: types_enum_values _11683B371707A009A1E2CDD66A84E38B carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum shader_effect_flags {
	_shader_effect_sort_bias_bit = 0,
	_shader_effect_uses_nonlinear_tint_bit = 1,
	_shader_effect_flags_dont_overdraw_first_person_weapon_bit = 2,
	NUMBER_OF_SHADER_EFFECT_FLAGS = 3,
};

#endif /* SHADER_EFFECT_FLAGS_H */

// shader_effect_flags: DB-verified via types_enum_values _11683B371707A009A1E2CDD66A84E38B
