#pragma once
/* particle_system_type_particle_state_interpolated_randomized_variables — randomized per-particle-state
 * appearance/motion multipliers (particle_systems.c). */

#include "real_argb_color.h"

typedef struct particle_system_type_particle_state_interpolated_randomized_variables
{
    float           scale;          /* 0x00 */
    float           animation_rate; /* 0x04 */
    float           rotation_rate;  /* 0x08 */
    real_argb_color color;          /* 0x0C */
} particle_system_type_particle_state_interpolated_randomized_variables;   /* 28 bytes */
