#pragma once

#include "unit_hud_state.h"  /* unit_hud_state now has a home; embedded by value */

typedef struct unit_hud_globals_definition
{
    unit_hud_state  hud_states[2]; /* 0x00 (176 bytes) */
    int             script_flags;  /* 0xB0 */
} unit_hud_globals_definition;     /* 180 bytes */

extern unit_hud_globals_definition *unit_hud_globals;
