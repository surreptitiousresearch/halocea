#pragma once

/* weapon_datum — runtime datum for a weapon-class object. It is the item-object datum layout:
 * a definition index followed by the by-value embedded object / item / weapon runtime payloads.
 * Layout from the database (832 bytes); matches C:/noav/headers_ref/weapon_datum.h. */

#include "_object_datum.h"
#include "_item_datum.h"
#include "_weapon_datum.h"

typedef struct weapon_datum
{
    int           definition_index;  /* 0x000 */
    _object_datum object;            /* 0x004 */
    _item_datum   item;              /* 0x1F4 */
    _weapon_datum weapon;            /* 0x22C */
} weapon_datum;                      /* 832 bytes */
