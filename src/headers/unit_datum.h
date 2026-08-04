#pragma once
/* unit_datum — a live unit object datum: a 4-byte definition index, the shared object payload
 * (_object_datum) followed by the unit-specific payload (_unit_datum). Composite layout DB-verified
 * (definition_index @0, _object_datum object @4, _unit_datum unit @500; 1228 total). The unit-level
 * field block lives in _unit_datum.h; the object-level block in _object_datum.h. */

#include "_object_datum.h"
#include "_unit_datum.h"

typedef struct unit_datum
{
    int           definition_index;          /* 0x000 */
    _object_datum object;                    /* 0x004 */
    _unit_datum   unit;                      /* 0x1F4 (500) */
} unit_datum;                                /* 1228 bytes */
