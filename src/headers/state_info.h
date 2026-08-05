#pragma once
/* state_info — one resolved slot of a HUD state message (4 bytes). Either an icon element pointer,
 * or a text reference (string index + scenario-names flag). Layout from the database. */

#include <stdint.h>
#include "icon_hud_element_definition.h"

/* IDA anonymous struct for the text arm (DB: _BECA036B426CABFAF46B8904C2CEA3F4, 4 bytes) */
typedef struct _BECA036B426CABFAF46B8904C2CEA3F4
{
    int16_t       string_index;            /* 0x0 */
    unsigned char uses_scenario_names;     /* 0x2 */
} _BECA036B426CABFAF46B8904C2CEA3F4;

typedef union state_info
{
    const icon_hud_element_definition *icon;         /* 0x0 */
    struct _BECA036B426CABFAF46B8904C2CEA3F4 text;   /* 0x0 */
} state_info;                                        /* 0x4 (4 bytes) */
