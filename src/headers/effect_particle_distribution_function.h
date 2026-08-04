#ifndef EFFECT_PARTICLE_DISTRIBUTION_FUNCTION_H
#define EFFECT_PARTICLE_DISTRIBUTION_FUNCTION_H

/* effect_particle_distribution_function — the count-distribution shape of an effect particle
 * definition (effect_particles_definition.distribution_function @0x68), selecting how a part's
 * particle output is spread across its event's duration. Consumed by
 * effect_evaluate_function_integral, whose per-case integral matches each name:
 *   start    -> integral is 1 everywhere (all output emitted at the start)
 *   end      -> integral is 1 only at fraction>=1 (all output emitted at the end)
 *   constant -> integral == fraction (constant emission rate)
 *   buildup  -> integral == f*f (back-loaded)
 *   falloff  -> integral == (2-f)*f (front-loaded)
 *   quadratic-> integral == smoothstep(f)
 * DB-verified: types_enum_values _FF8D495735AD1C58EDC19AD95EB6CCA2 carries these names verbatim. */
enum effect_particle_distribution_function
{
    _effect_particle_distribution_function_start     = 0,
    _effect_particle_distribution_function_end       = 1,
    _effect_particle_distribution_function_constant   = 2,
    _effect_particle_distribution_function_buildup    = 3,
    _effect_particle_distribution_function_falloff    = 4,
    _effect_particle_distribution_function_quadratic  = 5,
    NUMBER_OF_EFFECT_PARTICLE_DISTRIBUTION_FUNCTIONS  = 6,
};

#endif /* EFFECT_PARTICLE_DISTRIBUTION_FUNCTION_H */
