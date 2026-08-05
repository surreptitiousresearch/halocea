#pragma once
/* particle_system_type_state — one emitter state within a particle system type's type_states block
 * (192 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "tag_block.h"
#include "particle_system_type_state_interpolated_variables.h"

typedef struct particle_system_type_state
{
    char      name[32];                    /* 0x00 */
    float     duration_lower_bound;        /* 0x20 */
    float     duration_upper_bound;        /* 0x24 */
    float     transition_time_lower_bound; /* 0x28 */
    float     transition_time_upper_bound; /* 0x2C */
    unsigned int flags;                    /* 0x30 */
    particle_system_type_state_interpolated_variables variables; /* 0x34 */
    int       unused[21];                  /* 0x5C */
    int16_t   particle_creation_physics;   /* 0xB0 */
    int16_t   particle_update_physics;     /* 0xB2 */
    tag_block physics_constants;           /* 0xB4 */
} particle_system_type_state;              /* 192 bytes */
