#ifndef GLOW_PARTICLE_DISTRIBUTION_TYPE_H
#define GLOW_PARTICLE_DISTRIBUTION_TYPE_H

/* glow_particle_distribution_type — glow_definition.normal_particle_distribution /
 * trailing_particle_distribution: how a glow particle's initial lifetime cursor (t) is
 * seeded across the emission.
 * DB anonymous enum _1E65EF3947D5D690FC1682970AEAF22A (types_enum_values). */
enum glow_particle_distribution_type
{
	_glow_particle_distribution_type_random  = 0,   /* t uniform-random over total_time */
	_glow_particle_distribution_type_uniform = 1,   /* t = (index/total) * total_time */
	NUMBER_OF_NORMAL_PARTICLE_DISTRIBUTION_TYPES = 2,
};

#endif /* GLOW_PARTICLE_DISTRIBUTION_TYPE_H */
