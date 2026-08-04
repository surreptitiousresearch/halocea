#pragma once
/* weapon_hud_overlay_item — one state-driven overlay frame entry of a weapon_hud_overlay_definition's
 * `items` tag_block (hud.c). Layout from the database (136 bytes). */

#include "hud_placement_definition.h"
#include "hud_color_definition.h"

typedef struct weapon_hud_overlay_item
{
    hud_placement_definition placement;       /* 0x00 */
    hud_color_definition     colors;           /* 0x24 */
    __int16                  frame_rate;         /* 0x44 */
    __int16                  pad;                 /* 0x46 */
    __int16                  sequence_index;       /* 0x48 */
    __int16                  type;                  /* 0x4A */
    int                       flags;                 /* 0x4C */
    int                       unused[14];             /* 0x50 */
} weapon_hud_overlay_item;                             /* 0x88 (136 bytes) */
