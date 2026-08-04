#pragma once
/* weapon_hud_globals_definition — runtime weapon-HUD state (244 bytes). Reset wholesale to -1 on new map.
 * Layout from the database: two per-local-player weapon_hud_state blocks (flash timing), two crosshair
 * runtime-state blocks, and a script flags word. */

#include "weapon_hud_globals_definition.h"  /* canonical home: type + weapon_hud_globals extern */
