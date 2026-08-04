#pragma once
/* _equipment_definition — the equipment-level payload ('eqip' tag, 168 bytes) at offset 0x308 inside
 * the composite equipment_definition, after _object_definition (0x00) and _item_definition (0x17C).
 * Layout DB-verified against type '_equipment_definition' (2026-07-14). */

#include "tag_reference.h"

typedef struct _equipment_definition
{
    __int16       powerup_type;      /* 0x00 */
    __int16       grenade_type;      /* 0x02 */
    float         powerup_duration;  /* 0x04 */
    tag_reference pickup_sound;      /* 0x08 */
    unsigned int  unused[36];        /* 0x18 */
} _equipment_definition;             /* 168 bytes */
