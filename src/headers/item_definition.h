#pragma once
/* item_definition — the 'item' tag body: the shared object payload followed by the item payload
   (776 bytes; layout from the database). Weapon/equipment/garbage definitions extend this. */

#include "_object_definition.h"
#include "_item_definition.h"

typedef struct item_definition
{
    _object_definition object;  /* 0x000 (380 bytes) */
    _item_definition   item;    /* 0x17C (396 bytes) */
} item_definition;              /* 776 bytes */
