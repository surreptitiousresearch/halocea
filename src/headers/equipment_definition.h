#pragma once
/* equipment_definition — composite 'eqip' tag definition: shared object level, item level, then the
 * equipment-level payload. Composite layout DB-verified (object @0, item @380, equipment @776;
 * 944 bytes total, 2026-07-14). */

#include "_object_definition.h"
#include "_item_definition.h"
#include "_equipment_definition.h"

typedef struct equipment_definition
{
    _object_definition    object;    /* 0x000 */
    _item_definition      item;      /* 0x17C (380) */
    _equipment_definition equipment; /* 0x308 (776) */
} equipment_definition;              /* 944 bytes */
