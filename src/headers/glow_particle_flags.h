#ifndef GLOW_PARTICLE_FLAGS_H
#define GLOW_PARTICLE_FLAGS_H

/* glow_particle_flags - bit indices for the glow_particle.flags word.
 * DB-verified: types_enum_values _F7A6B1E2D56015BF3DF50E33777F50FD carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum glow_particle_flags {
	_glow_particle_traversal_direction_negative_bit = 0,
	_glow_particle_is_trailing_particle_bit = 1,
	NUMBER_OF_GLOW_PARTICLE_FLAGS = 2,
};

#endif /* GLOW_PARTICLE_FLAGS_H */

// glow_particle_flags: DB-verified via types_enum_values _F7A6B1E2D56015BF3DF50E33777F50FD
