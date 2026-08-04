#pragma once
/* biped_datum — the full object record for a biped (player/AI character). It is the object-header
 * datum payload for a biped object: a generic object datum, followed by the unit-class extension and
 * the biped-class extension. Total 1360 bytes (0x550), matching the size unit_control / movement code
 * memcpy's out of the object pool. object/unit reuse the fully DB-verified _object_datum/_unit_datum
 * types (see object_datum.h/unit_datum.h); _biped_datum is fully resolved (DB types_members-confirmed,
 * 132 bytes). */

#include "object_datum.h"
#include "unit_datum.h"
#include "_biped_datum.h"

typedef struct biped_datum
{
    int           definition_index;   /* 0x000 — biped tag definition index */
    _object_datum object;             /* 0x004 */
    _unit_datum   unit;               /* 0x1F4 */
    _biped_datum  biped;              /* 0x4CC */
} biped_datum;                        /* 0x550 bytes */
