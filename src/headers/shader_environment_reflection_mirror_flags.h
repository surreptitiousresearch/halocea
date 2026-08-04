#ifndef SHADER_ENVIRONMENT_REFLECTION_MIRROR_FLAGS_H
#define SHADER_ENVIRONMENT_REFLECTION_MIRROR_FLAGS_H

/* shader_environment_reflection_mirror_flags - bit indices for the shader_environment_reflection_mirror.flags word.
 * DB-verified: types_enum_values _D5B343711980D825F229DFA3D407835A carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_environment_reflection_mirror_flags {
	_shader_environment_reflection_mirror_bit = 0,
	NUMBER_OF_SHADER_ENVIRONMENT_REFLECTION_FLAGS = 1,
};

#endif /* SHADER_ENVIRONMENT_REFLECTION_MIRROR_FLAGS_H */

// shader_environment_reflection_mirror_flags: DB-verified via types_enum_values _D5B343711980D825F229DFA3D407835A
