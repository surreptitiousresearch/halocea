#pragma once
/* breakable_surface — the breakable-surface sub-block of a material_definition tag (84 bytes).
 * Layout from the database. */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct breakable_surface
{
    float         maximum_vitality;   /* 0x00 */
    int           unused1[2];         /* 0x04 */
    unsigned int  flags;              /* 0x0C */
    tag_reference effect;             /* 0x10 */
    tag_reference sound;              /* 0x20 */
    int           unused2[6];         /* 0x30 */
    tag_block     particle_effects;   /* 0x48 */
} breakable_surface;                  /* 84 bytes */
