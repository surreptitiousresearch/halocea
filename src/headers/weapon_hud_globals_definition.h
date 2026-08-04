#pragma once

#include "weapon_hud_state.h"
#include "crosshair_hud_state.h"

typedef struct weapon_hud_globals_definition
{
    weapon_hud_state    hud_state[2];        /* 0x00 — 80 bytes */
    crosshair_hud_state crosshair_state[2];  /* 0x50 — 160 bytes */
    int                 script_flags;        /* 0xF0 */
} weapon_hud_globals_definition;             /* 244 bytes */

extern weapon_hud_globals_definition *weapon_hud_globals;
