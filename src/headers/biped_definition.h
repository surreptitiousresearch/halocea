#pragma once
/* biped_definition (tag) — a biped's parsed tag definition (1268 bytes). Composed of the shared object
 * definition, the unit definition, and the biped-specific block. */

#include "_object_definition.h"
#include "_unit_definition.h"
#include "_biped_definition.h"

typedef struct biped_definition
{
    _object_definition object;      /* 0x000 */
    _unit_definition   unit;        /* 0x17C */
    _biped_definition  biped;       /* 0x2F0 */
} biped_definition;                 /* 1268 bytes */
