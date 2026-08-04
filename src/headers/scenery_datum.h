#pragma once
/* scenery_datum — a live scenery object datum stored in object_header_data's pool: the shared object payload
 * followed by the scenery-specific payload. Layout from the database (types_members, 504 bytes). */

#include "object_datum.h"
#include "_scenery_datum.h"

typedef struct scenery_datum
{
    int            definition_index;  /* 0x000 */
    _object_datum  object;            /* 0x004 */
    _scenery_datum scenery;           /* 0x1F4 (500) */
} scenery_datum;                      /* 504 bytes */
