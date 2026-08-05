#pragma once
/* structure_fog_palette_entry — one element of structure_bsp.fog_palette (136 bytes; DB-verified
 * via types_members). Names a fog tag and a runtime global-function binding. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct structure_fog_palette_entry
{
    char             name[32];                     /* 0x00 */
    tag_reference    fog;                           /* 0x20 */
    uint16_t pad;                           /* 0x30 */
    int16_t          runtime_global_function_index; /* 0x32 */
    char             global_function_name[32];      /* 0x34 */
    int              unused[13];                    /* 0x54 */
} structure_fog_palette_entry;                      /* 136 bytes */
