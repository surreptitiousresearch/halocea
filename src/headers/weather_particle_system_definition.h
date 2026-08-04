#pragma once
/* weather_particle_system_definition — weather tag definition (48 bytes; DB-verified). */

#include "tag_block.h"

typedef struct weather_particle_system_definition
{
    unsigned int flags;             /* 0x00 */
    int          unused[8];         /* 0x04 */
    tag_block    particle_types;    /* 0x24 */
} weather_particle_system_definition;   /* 48 bytes */
