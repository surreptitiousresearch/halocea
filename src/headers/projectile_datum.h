#pragma once
/* projectile_datum — a live projectile object datum stored in object_header_data's pool: the shared object
 * payload, the item-layer payload, then the projectile-specific payload. Layout from the database
 * (688 bytes). */

#include "object_datum.h"
#include "_item_datum.h"
#include "_projectile_datum.h"

typedef struct projectile_datum
{
    int               definition_index;  /* 0x000 */
    _object_datum     object;            /* 0x004 */
    _item_datum       item;              /* 0x1F4 (500) */
    _projectile_datum projectile;        /* 0x22C (556) */
} projectile_datum;                      /* 688 bytes */
