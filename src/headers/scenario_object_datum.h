#pragma once
/* scenario_object_datum — one scenario object placement (palette-indexed). Complete: the database
 * models this type as exactly these 9 members (40 bytes; DB types/types_members, size 40). */

#include "real_point3d.h"
#include "real_euler_angles3d.h"

typedef struct scenario_object_datum
{
    __int16             palette_entry_index; /* 0x00 */
    __int16             name_index;          /* 0x02 */
    unsigned __int16    placement_flags;     /* 0x04 */
    __int16             variant_number;      /* 0x06 */
    real_point3d        position;            /* 0x08 */
    real_euler_angles3d rotation;            /* 0x14 */
    unsigned __int16    on_bsp_flags;        /* 0x20 — bit per structure BSP this placement belongs to */
    unsigned __int16    misc_flags;          /* 0x22 */
    unsigned int        unused;              /* 0x24 */
} scenario_object_datum;                     /* 40 bytes */
