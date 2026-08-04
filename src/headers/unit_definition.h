#pragma once
/* unit_definition — the unit tag definition. The nested unit block is the shared `_unit_definition`
 * (372 bytes, DB types_members; body in _unit_definition.h); the leading `_object_definition`
 * (380 bytes) is the shared header every object-derived tag begins with. */

#include "_object_definition.h"
#include "_unit_definition.h"

typedef struct unit_definition
{
    _object_definition object;  /* 0x000 */
    _unit_definition   unit;    /* 0x17C (380) */
} unit_definition;              /* 752 bytes */
