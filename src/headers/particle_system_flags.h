#ifndef PARTICLE_SYSTEM_FLAGS_H
#define PARTICLE_SYSTEM_FLAGS_H

/* particle_system_flags - bit indices for the particle_system.flags word.
 * DB-verified: types_enum_values _B480F3D5302DDD26DD58C5FA84DA868D carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum particle_system_flags {
	_particle_system_active_bit = 0,
	_particle_system_initializing_bit = 1,
	NUMBER_OF_PARTICLE_SYSTEM_FLAGS = 2,
};

#endif /* PARTICLE_SYSTEM_FLAGS_H */

// particle_system_flags: DB-verified via types_enum_values _B480F3D5302DDD26DD58C5FA84DA868D
