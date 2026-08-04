#pragma once
/* particle_system_type_state_interpolated_variables — the per-type interpolated emitter variables for
 * the current type state (particle_systems.c). */

#include "particle_system_type_particle_state_interpolated_randomized_variables.h"
#include "particle_system_type_particle_state_interpolated_variables.h"

typedef struct particle_system_type_state_interpolated_variables
{
    particle_system_type_particle_state_interpolated_randomized_variables particle_state_randomized_multipliers; /* 0x00 */
    particle_system_type_particle_state_interpolated_variables            particle_state_multipliers;            /* 0x1C */
    float minimum_particle_count;  /* 0x20 */
    float particle_creation_rate;  /* 0x24 */
} particle_system_type_state_interpolated_variables;   /* 40 bytes */
