#pragma once
/* particle_system_type — one emitter type within a particle system definition tag (particle_systems.c).
 * 128 bytes; layout from the database. */

#include <stdint.h>
#include "tag_block.h"
#include "particle_system_type_particle_state_interpolated_variables.h"

typedef struct particle_system_type
{
    char             name[32];                          /* 0x00 */
    unsigned int     flags;                             /* 0x20 */
    int16_t          initial_particle_count;            /* 0x24 */
    uint16_t pad;                               /* 0x26 */
    int16_t          complex_sprite_render_mode;        /* 0x28 */
    int16_t          sprite_render_mode;                /* 0x2A */
    particle_system_type_particle_state_interpolated_variables variables; /* 0x2C */
    int              unused[9];                         /* 0x30 */
    int16_t          initial_particle_creation_physics; /* 0x54 */
    uint16_t pad2;                              /* 0x56 */
    unsigned int     physics_flags;                     /* 0x58 */
    tag_block        physics_constants;                 /* 0x5C */
    tag_block        type_states;                       /* 0x68 — 192-byte particle_system_type_state */
    tag_block        particle_states;                   /* 0x74 — 376-byte particle states */
} particle_system_type;                                 /* 128 bytes */
