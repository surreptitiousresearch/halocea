#ifndef SHADER_ENVIRONMENT_DIFFUSE_RESCALE_FLAGS_H
#define SHADER_ENVIRONMENT_DIFFUSE_RESCALE_FLAGS_H

/* shader_environment_diffuse_rescale_flags - bit indices for the shader_environment_diffuse_rescale.flags word.
 * DB-verified: types_enum_values _93C77EBBF3DA450D024A85AA8BE93119 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_environment_diffuse_rescale_flags {
	_shader_environment_diffuse_rescale_detail_maps_bit = 0,
	_shader_environment_diffuse_rescale_bump_map_bit = 1,
	NUMBER_OF_SHADER_ENVIRONMENT_DIFFUSE_FLAGS = 2,
};

#endif /* SHADER_ENVIRONMENT_DIFFUSE_RESCALE_FLAGS_H */

// shader_environment_diffuse_rescale_flags: DB-verified via types_enum_values _93C77EBBF3DA450D024A85AA8BE93119
