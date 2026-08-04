#pragma once
/* particle_system_definition — a 'pctl' tag: system-wide physics plus the emitter types block
 * (104 bytes, DB types_members-confirmed). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct particle_system_definition
{
    unsigned int  flags;                       /* 0x00 */
    int           unused[13];                  /* 0x04 */
    tag_reference system_update_point_physics; /* 0x38 */
    __int16       system_update_physics;       /* 0x48 */
    unsigned __int16 pad;                      /* 0x4A */
    unsigned int  system_physics_flags;        /* 0x4C */
    tag_block     physics_constants;           /* 0x50 */
    tag_block     types;                       /* 0x5C */
} particle_system_definition;                  /* 104 bytes */
