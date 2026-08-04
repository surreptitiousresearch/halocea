#pragma once
/* weapon_hud_crosshair_definition — one crosshair's bitmap + overlay-items block, embedded by value
 * in weapon_hud_crosshairs_element.crosshairs (hud.c). DB-verified layout (types_members). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct weapon_hud_crosshair_definition
{
    tag_reference bitmap;   /* 0x00 — crosshair bitmap group */
    tag_block     items;    /* 0x10 — weapon_hud_crosshair_item, stride 108 */
} weapon_hud_crosshair_definition;   /* 0x1C = 28 bytes */
