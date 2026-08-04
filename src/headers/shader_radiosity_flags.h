#ifndef SHADER_RADIOSITY_FLAGS_H
#define SHADER_RADIOSITY_FLAGS_H

/* shader_radiosity_flags - bit indices for the shader_radiosity.flags word.
 * DB-verified: types_enum_values _754D81C89DC3B67DF71EEB98FCCA5137 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_radiosity_flags {
	_shader_radiosity_simple_parameterization_bit = 0,
	_shader_radiosity_ignore_normals_bit = 1,
	_shader_radiosity_FILTHY_transparent_lit_bit = 2,
	NUMBER_OF_SHADER_RADIOSITY_FLAGS = 3,
};

#endif /* SHADER_RADIOSITY_FLAGS_H */

// shader_radiosity_flags: DB-verified via types_enum_values _754D81C89DC3B67DF71EEB98FCCA5137
