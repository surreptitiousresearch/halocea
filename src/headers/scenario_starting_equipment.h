#ifndef SCENARIO_STARTING_EQUIPMENT_H
#define SCENARIO_STARTING_EQUIPMENT_H

/* scenario_starting_equipment — one element of scenario.starting_equipment (204-byte stride).
 * Layout from the IDA DB type `scenario_starting_equipment` (types_members). Each item_collection
 * slot is a tag_reference to an item-collection tag granted on spawn. */

#include "tag_reference.h"

typedef struct scenario_starting_equipment
{
    int           flags;              /* 0x00 */
    __int16       game_type[4];       /* 0x04 */
    int           unused1[12];        /* 0x0C */
    tag_reference item_collection[6]; /* 0x3C */
    int           unused2[12];        /* 0x9C */
} scenario_starting_equipment;        /* 204 bytes */

#endif /* SCENARIO_STARTING_EQUIPMENT_H */
