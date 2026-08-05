#pragma once
/* structure_background_sound_palette_entry — one element of structure_bsp.background_sound_palette
 * (116 bytes; DB-verified via types_members). Names a background-sound tag and a runtime
 * global-function binding. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct structure_background_sound_palette_entry
{
    char             name[32];                     /* 0x00 */
    tag_reference    background_sound;             /* 0x20 */
    uint16_t pad;                           /* 0x30 */
    int16_t          runtime_global_function_index; /* 0x32 */
    char             global_function_name[32];      /* 0x34 */
    int              unused[8];                     /* 0x54 */
} structure_background_sound_palette_entry;         /* 116 bytes */
