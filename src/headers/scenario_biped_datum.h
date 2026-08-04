#pragma once
/* scenario_biped_datum — one biped scenario placement (120 bytes, DB types_members-confirmed layout). */

#include "scenario_object_datum.h"
#include "scenario_object_permutation.h"
#include "scenario_unit_datum.h"

typedef struct scenario_biped_datum
{
    scenario_object_datum       object;      /* 0x00 */
    scenario_object_permutation permutation; /* 0x28 */
    scenario_unit_datum         unit;        /* 0x48 */
    int                         unused[8];   /* 0x58 */
} scenario_biped_datum; /* 120 bytes */
