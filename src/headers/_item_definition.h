#pragma once
/* _item_definition — the shared item tag payload (396 bytes) that every item-derived definition
 * (weapon, equipment, garbage, ...) carries after its _object_definition. Layout is authoritative:
 * matches both C:\noav\headers_ref\_item_definition.h and the PDB types_members dump for
 * `_item_definition` (sizeof 0x18C). */

#include "tag_reference.h"

typedef struct _item_definition
{
    unsigned int    flags;                                  /* 0x00 */
    __int16         hud_message_index;                       /* 0x04 */
    __int16         inventory_sort_order;                     /* 0x06 */
    float           scale;                                     /* 0x08 */
    __int16         hud_message_value_scale;                    /* 0x0C */
    __int16         padd;                                        /* 0x0E */
    int             unused[4];                                    /* 0x10 */
    __int16         function_modes[4];                             /* 0x20 */
    int             inventory_unused[41];                          /* 0x28 */
    tag_reference   material_effects;                              /* 0xCC */
    tag_reference   collision_sound;                               /* 0xDC */
    int             unused2[30];                                   /* 0xEC */
    float           detonation_delay_timer_lower_bound;            /* 0x164 */
    float           detonation_delay_timer_upper_bound;            /* 0x168 */
    tag_reference   detonating_effect;                             /* 0x16C */
    tag_reference   detonation_effect;                             /* 0x17C */
} _item_definition; /* 0x18C = 396 bytes */
