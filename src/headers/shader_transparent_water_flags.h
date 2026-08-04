#ifndef SHADER_TRANSPARENT_WATER_FLAGS_H
#define SHADER_TRANSPARENT_WATER_FLAGS_H

/* shader_transparent_water_flags - bit indices for the shader_transparent_water.flags word.
 * DB-verified: types_enum_values _DC8602BDD59BD6E235294DAFCB74CC44 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_transparent_water_flags {
	_shader_transparent_water_base_map_alpha_modulates_reflection_bit = 0,
	_shader_transparent_water_base_map_color_modulates_background_bit = 1,
	_shader_transparent_water_atmospheric_fog_bit = 2,
	_shader_transparent_water_draw_before_fog_bit = 3,
	NUMBER_OF_SHADER_TRANSPARENT_WATER_FLAGS = 4,
};

#endif /* SHADER_TRANSPARENT_WATER_FLAGS_H */

// shader_transparent_water_flags: DB-verified via types_enum_values _DC8602BDD59BD6E235294DAFCB74CC44
