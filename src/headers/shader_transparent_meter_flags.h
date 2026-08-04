#ifndef SHADER_TRANSPARENT_METER_FLAGS_H
#define SHADER_TRANSPARENT_METER_FLAGS_H

/* shader_transparent_meter_flags - bit indices for the shader_transparent_meter.flags word.
 * DB-verified: types_enum_values _7DB9E68EAD7EA80D8E9D3578955ABFD7 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_transparent_meter_flags {
	_shader_transparent_meter_decal_bit = 0,
	_shader_transparent_meter_two_sided_bit = 1,
	_shader_transparent_meter_flash_color_is_negative_bit = 2,
	_shader_transparent_meter_tint_mode_2_bit = 3,
	_shader_transparent_meter_point_sampled_bit = 4,
	NUMBER_OF_SHADER_TRANSPARENT_METER_FLAGS = 5,
};

#endif /* SHADER_TRANSPARENT_METER_FLAGS_H */

// shader_transparent_meter_flags: DB-verified via types_enum_values _7DB9E68EAD7EA80D8E9D3578955ABFD7
