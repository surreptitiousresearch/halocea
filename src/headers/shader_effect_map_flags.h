#ifndef SHADER_EFFECT_MAP_FLAGS_H
#define SHADER_EFFECT_MAP_FLAGS_H

/* shader_effect_map_flags - bit indices for the shader_effect_map.flags word.
 * DB-verified: types_enum_values _89331D359CDD4C469232CE26BBF60BBB carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum shader_effect_map_flags {
	_shader_effect_map_point_sampled_bit = 0,
	NUMBER_OF_SHADER_EFFECT_MAP_USER_FLAGS = 1,
	_shader_effect_map_u_clamp_bit = 1,
	_shader_effect_map_v_clamp_bit = 2,
	NUMBER_OF_SHADER_EFFECT_MAP_FLAGS = 3,
};

#endif /* SHADER_EFFECT_MAP_FLAGS_H */

// shader_effect_map_flags: DB-verified via types_enum_values _89331D359CDD4C469232CE26BBF60BBB
