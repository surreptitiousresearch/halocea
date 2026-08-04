#pragma once
/* scenario_equipment_datum — scenario placement block for an equipment object (DB types_members-confirmed,
 * 40 bytes — just the shared object header, no equipment-specific fields). */

#include "scenario_object_datum.h"

typedef struct scenario_equipment_datum
{
    scenario_object_datum object; /* 0x00 */
} scenario_equipment_datum; /* 40 bytes */
