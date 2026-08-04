#pragma once
/* item_datum — a live item-class object datum (weapon/equipment/garbage base shape without the
 * class-specific tail): 4-byte definition index, shared object payload, item payload.
 * DB-verified (definition_index @0, _object_datum object @4 (496), _item_datum item @500 (56); 556 total). */

#include "_object_datum.h"
#include "_item_datum.h"

typedef struct item_datum
{
    int           definition_index;   /* 0x000 */
    _object_datum object;             /* 0x004 */
    _item_datum   item;               /* 0x1F4 (500) */
} item_datum;                          /* 556 bytes */
