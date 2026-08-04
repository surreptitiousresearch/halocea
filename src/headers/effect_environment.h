#ifndef EFFECT_ENVIRONMENT_H
#define EFFECT_ENVIRONMENT_H

/* effect_environment — the environment restriction on an effect part / particle
 * (effect_part_definition.environment, effect_particles_definition.environment). Gates whether the
 * part may spawn at a world point given whether that point is underwater.
 * DB-verified: types_enum_values _CBCB66E913ADDA3C4A435C8A89DE0BFA carries these names verbatim. */
enum effect_environment
{
    _effect_environment_anywhere = 0,
    _effect_environment_air      = 1,
    _effect_environment_water    = 2,
    _effect_environment_vacuum   = 3,
    NUMBER_OF_EFFECT_ENVIRONMENTS = 4,
};

#endif /* EFFECT_ENVIRONMENT_H */
