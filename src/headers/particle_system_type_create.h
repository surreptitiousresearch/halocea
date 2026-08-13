#pragma once

/* particle_system_type_create — the creation-physics selector of a particle_system_type
 * (tag fields initial_particle_creation_physics / particle_creation_physics), and the index
 * domain of the particle_creation_functions[3] dispatch table
 * (particle_system_new_particles.c). No literal site exists — the index always arrives from
 * tag data — so this header names the domain the dispatch and the two creation TUs
 * (particle_system_new_particle_explosion/jet.c) already describe in prose.
 * Source: types_enum_values $B8C3348E47B1DEBD43D1DF01223F0334 (DB-verified, verbatim). */
enum particle_system_type_create {
	_particle_system_type_create_default = 0,
	_particle_system_type_create_explosion = 1,
	_particle_system_type_create_jet = 2,
	NUMBER_OF_PARTICLE_SYSTEM_TYPE_CREATION_PHYSICS = 3,
};

// particle_system_type_create: DB-verified via types_enum_values $B8C3348E47B1DEBD43D1DF01223F0334
