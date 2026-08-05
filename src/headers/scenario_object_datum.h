#pragma once
/* scenario_object_datum — one scenario object placement (palette-indexed). Complete: the database
 * models this type as exactly these 9 members (40 bytes; DB types/types_members, size 40). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_euler_angles3d.h"

typedef struct scenario_object_datum
{
    int16_t             palette_entry_index; /* 0x00 */
    int16_t             name_index;          /* 0x02 */
    uint16_t    placement_flags;     /* 0x04 */
    int16_t             variant_number;      /* 0x06 */
    real_point3d        position;            /* 0x08 */
    real_euler_angles3d rotation;            /* 0x14 */
    uint16_t    on_bsp_flags;        /* 0x20 — bit per structure BSP this placement belongs to */
    uint16_t    misc_flags;          /* 0x22 */
    unsigned int        unused;              /* 0x24 */
} scenario_object_datum;                     /* 40 bytes */
