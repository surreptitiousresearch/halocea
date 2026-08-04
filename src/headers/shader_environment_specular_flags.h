#ifndef SHADER_ENVIRONMENT_SPECULAR_FLAGS_H
#define SHADER_ENVIRONMENT_SPECULAR_FLAGS_H

/* shader_environment_specular_flags - bit indices for the shader_environment_specular.flags word.
 * DB-verified: types_enum_values _F52BA0B413EFD074DA4AD7F5D03BC4D0 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_environment_specular_flags {
	_shader_environment_specular_overbright_bit = 0,
	_shader_environment_specular_extra_shiny_bit = 1,
	_shader_environment_specular_lightmap_bit = 2,
	NUMBER_OF_SHADER_ENVIRONMENT_SPECULAR_FLAGS = 3,
};

#endif /* SHADER_ENVIRONMENT_SPECULAR_FLAGS_H */

// shader_environment_specular_flags: DB-verified via types_enum_values _F52BA0B413EFD074DA4AD7F5D03BC4D0
