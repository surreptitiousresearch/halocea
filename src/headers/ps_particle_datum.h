#pragma once
/* ps_particle_datum — one particle owned by a particle system (particle_systems.c). 128 bytes; layout
 * from the database. Distinct from particle_datum (the standalone particle pool). */

#include <stdint.h>
#include "location.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "particle_system_type_particle_state_interpolated_randomized_variables.h"

typedef struct ps_particle_datum
{
    int16_t          identifier;             /* 0x00 */
    uint8_t  states_moving_forward;  /* 0x02 */
    uint8_t  valid;                  /* 0x03 */
    int              next_particle_index;    /* 0x04 */
    int16_t          state_index;            /* 0x08 — 0xFFFF when dead */
    int16_t          transition_state_index; /* 0x0A */
    float            time_left_in_state;     /* 0x0C */
    float            state_length;           /* 0x10 */
    location         location;               /* 0x14 */
    real_point3d     position;               /* 0x1C */
    real_vector3d    velocity;               /* 0x28 */
    real_vector3d    axis;                   /* 0x34 */
    float            rotation;               /* 0x40 */
    float            sprite_index;           /* 0x44 */
    particle_system_type_particle_state_interpolated_randomized_variables randomized_variables;            /* 0x48 */
    particle_system_type_particle_state_interpolated_randomized_variables transition_randomized_variables; /* 0x64 */
} ps_particle_datum;                         /* 128 bytes */
