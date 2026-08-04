#pragma once
/* scenario_vehicle_datum — one vehicle scenario placement (120 bytes, DB types_members-confirmed layout). */

#include "scenario_object_datum.h"
#include "scenario_object_permutation.h"
#include "scenario_unit_datum.h"

typedef struct scenario_vehicle_datum
{
    scenario_object_datum       object;                  /* 0x00 */
    scenario_object_permutation permutation;              /* 0x28 */
    scenario_unit_datum         unit;                     /* 0x48 */
    unsigned __int8             multiplayer_team_index;   /* 0x58 */
    unsigned __int8             unused_byte;              /* 0x59 */
    unsigned __int16            multiplayer_spawn_flags;  /* 0x5A */
    int                         unused[7];                /* 0x5C */
} scenario_vehicle_datum; /* 120 bytes */
