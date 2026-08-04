#ifndef EFFECT_PARTICLE_FLAGS_H
#define EFFECT_PARTICLE_FLAGS_H

/* effect_particle_flags - bit indices for the effect_particle.flags word.
 * DB-verified: types_enum_values _71D59322AAC4D8362679055A3E1FF7F4 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum effect_particle_flags {
	_effect_particle_count_bit = 5,
	_effect_particle_count_delta_bit = 6,
	_effect_particle_distribution_radius_bit = 7,
	_effect_particle_distribution_radius_delta_bit = 8,
	_effect_particle_radius_bit = 9,
	_effect_particle_radius_delta_bit = 10,
	_effect_particle_tint_bit = 11,
	NUMBER_OF_EFFECT_PARTICLES_SCALEABLE_VALUES = 12,
};

#endif /* EFFECT_PARTICLE_FLAGS_H */

// effect_particle_flags: DB-verified via types_enum_values _71D59322AAC4D8362679055A3E1FF7F4
