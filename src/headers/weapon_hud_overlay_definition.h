#pragma once
/* weapon_hud_overlay_definition — a set of flag/state-driven overlay bitmap frames drawn on top of a
 * weapon or grenade HUD panel (hud.c). Layout from the database (28 bytes). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct weapon_hud_overlay_definition
{
    tag_reference bitmap; /* 0x00 */
    tag_block     items;  /* 0x10 */
} weapon_hud_overlay_definition; /* 0x1C (28 bytes) */
