#pragma once
/* scenario_weapon_datum — a placed weapon in the scenario weapon palette (92 bytes, DB layout). */

#include "scenario_object_datum.h"
#include "scenario_object_permutation.h"

typedef struct scenario_weapon_datum
{
    scenario_object_datum       object;         /* 0x00 */
    scenario_object_permutation permutation;    /* 0x28 */
    __int16                     rounds_total;    /* 0x48 */
    __int16                     rounds_loaded;   /* 0x4A */
    unsigned __int16            flags;           /* 0x4C */
    unsigned __int16            pad;             /* 0x4E */
    unsigned int                unused[3];       /* 0x50 */
} scenario_weapon_datum; /* 92 bytes */
