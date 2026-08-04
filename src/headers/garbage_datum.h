#pragma once
/* garbage_datum — a live garbage object datum stored in object_header_data's pool: the shared object
 * payload, the item-layer payload, then the garbage-specific despawn-timer payload. Layout from the
 * database (580 bytes). */

#include "object_datum.h"
#include "_item_datum.h"
#include "_garbage_datum.h"

typedef struct garbage_datum
{
    int           definition_index;  /* 0x000 */
    _object_datum object;            /* 0x004 */
    _item_datum   item;              /* 0x1F4 (500) */
    _garbage_datum garbage;          /* 0x22C (556) */
} garbage_datum;                     /* 580 bytes */
