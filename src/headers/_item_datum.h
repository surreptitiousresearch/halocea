#pragma once
/* _item_datum — the item-layer runtime payload shared by item-class objects (weapons, equipment, garbage,
 * projectiles), embedded after _object_datum in their datum structs. Layout from the database (56 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct _item_datum
{
    unsigned int  flags;                      /* 0x00 */
    __int16       detonation_ticks;           /* 0x04 */
    __int16       rested_surface_index;       /* 0x06 */
    __int16       bsp_index;                  /* 0x08 */
    __int16       pad;                        /* 0x0A */
    int           ignore_object_index;        /* 0x0C */
    int           last_owned_time;            /* 0x10 */
    int           item_on_rest_object_index;  /* 0x14 */
    real_point3d  item_rest_object_offset;    /* 0x18 */
    real_vector3d rotation_axis;              /* 0x24 */
    float         rotation_sine;              /* 0x30 */
    float         rotation_cosine;            /* 0x34 */
} _item_datum;                                /* 56 bytes */
