#pragma once

#include "scenario_object_datum.h"
#include "scenario_object_permutation.h"

typedef struct scenario_scenery_datum
{
    scenario_object_datum object;           /* 0x00 */
    scenario_object_permutation permutation; /* 0x28 */
} scenario_scenery_datum; /* 72 bytes */
