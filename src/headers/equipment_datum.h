#pragma once
/* equipment_datum — a live equipment object datum stored in object_header_data's pool: the shared object
 * payload, the item-layer payload, then the equipment-specific payload. Layout from the database
 * (660 bytes). */

#include "object_datum.h"
#include "_item_datum.h"
#include "_equipment_datum.h"

typedef struct equipment_datum
{
    int              definition_index;  /* 0x000 */
    _object_datum    object;            /* 0x004 */
    _item_datum      item;              /* 0x1F4 (500) */
    _equipment_datum equipment;         /* 0x22C (556) */
} equipment_datum;                      /* 660 bytes */
