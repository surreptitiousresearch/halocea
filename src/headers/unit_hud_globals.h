#pragma once
/* unit_hud_globals — runtime per-local-player HUD state for the unit HUD. The global pointer
 * unit_hud_globals points at a unit_hud_globals_definition holding one unit_hud_state per local
 * player (split-screen supports up to 2). Only the fields read by reconstructed callers are
 * named; the layout matches the database exactly. */

#include "unit_hud_state.h"
#include "unit_hud_globals_definition.h"

extern unit_hud_globals_definition *unit_hud_globals;
