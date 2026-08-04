#pragma once
/* scenario_control_datum — a scenario "control" (switch/button) object placement (DB types_members-confirmed,
 * 64 bytes). */

#include "scenario_object_datum.h"
#include "scenario_device_datum.h"

typedef struct scenario_control_datum
{
    scenario_object_datum  object;                          /* 0x00 */
    scenario_device_datum  device;                           /* 0x28 */
    unsigned int           flags;                            /* 0x30 */
    __int16                hud_override_string_list_index;   /* 0x34 */
    __int16                pad;                              /* 0x36 */
    unsigned int           unused[2];                        /* 0x38 */
} scenario_control_datum; /* 64 bytes */
