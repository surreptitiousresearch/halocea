#pragma once
/* particle_type — runtime per-emitter-type state within a particle system datum (particle_systems.c). */

#include "particle_system_type_state_interpolated_variables.h"

typedef struct particle_type
{
    __int16 state_index;              /* 0x00 — 0xFFFF when inactive */
    __int16 transition_state_index;   /* 0x02 — 0xFFFF when not transitioning */
    float   time_left_in_state;       /* 0x04 */
    float   state_length;             /* 0x08 */
    particle_system_type_state_interpolated_variables variables; /* 0x0C */
    float   fractional_particle_count;/* 0x34 */
    unsigned __int8 states_moving_forward; /* 0x38 */
    unsigned __int8 pad;              /* 0x39 */
    __int16 particle_count;           /* 0x3A */
    int     first_particle_index;     /* 0x3C — head of this type's particle list */
} particle_type;                      /* 64 bytes */
