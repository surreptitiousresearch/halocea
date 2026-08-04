#pragma once
/* structure_sound_environment_palette_entry — one element of structure_bsp.sound_environment_palette
 * (80 bytes; DB-verified via types_members). Names a sound_environment tag. */

#include "tag_reference.h"

typedef struct structure_sound_environment_palette_entry
{
    char          name[32];          /* 0x00 */
    tag_reference sound_environment; /* 0x20 */
    int           unused[8];         /* 0x30 */
} structure_sound_environment_palette_entry; /* 80 bytes */
