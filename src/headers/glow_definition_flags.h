#ifndef GLOW_DEFINITION_FLAGS_H
#define GLOW_DEFINITION_FLAGS_H

/* glow_definition_flags — bit indices for glow_definition.flags.
 * Source: reference enum _FBA009FC4E931E7EB8BDB36BF98978E1 (DB). Values are BIT INDICES — use as (1u << _bit). */
enum glow_definition_flags {
	_glow_interpolate_color_per_particle_bit = 0x0,
	_glow_particles_initial_direction_negative_bit = 0x1,
	_glow_particles_move_in_both_directions_bit = 0x2,
	_glow_trailing_particles_fade_over_time_bit = 0x3,
	_glow_trailing_particles_shrink_over_time_bit = 0x4,
	_glow_trailing_particles_slow_over_time_bit = 0x5,
	NUMBER_OF_GLOW_FLAGS = 0x6,
};

#endif /* GLOW_DEFINITION_FLAGS_H */

// glow_definition_flags: DB-verified via types_enum_values _FBA009FC4E931E7EB8BDB36BF98978E1 (anonymous compiled enum, names sans leading _)
