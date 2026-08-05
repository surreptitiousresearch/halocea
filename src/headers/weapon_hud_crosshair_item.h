#pragma once
/* weapon_hud_crosshair_item — one overlay frame entry of a weapon_hud_crosshairs_element's
 * crosshair_overlays tag_block (hud.c). DB type exists but its resolved layout is an empty stub (size 108
 * only); fields carved from the crosshairs_draw disasm (lhz 0x44/0x46, lwz 0x48, stride mulli 0x6C).
 * Near-twin of weapon_hud_overlay_item, which packs frame_rate/sequence_index/flags differently. */

#include <stdint.h>
#include "hud_placement_definition.h"
#include "hud_color_definition.h"

typedef struct weapon_hud_crosshair_item
{
    hud_placement_definition placement;      /* 0x00 */
    hud_color_definition     colors;         /* 0x24 */
    int16_t                  frame_rate;     /* 0x44 */
    int16_t                  sequence_index; /* 0x46 — also the direct bitmap index when flags bit 0x2 (not a sprite) */
    int                      flags;          /* 0x48 — 0x1 flashes-when-active, 0x2 not-a-sprite,
                                                       0x4 show-only-when-zoomed, 0x10 hide-area-outside-reticle,
                                                       0x20 one-zoom-level, 0x40 don't-show-when-zoomed,
                                                       0x80 disabled */
    int                      unused[8];      /* 0x4C */
} weapon_hud_crosshair_item;                 /* 0x6C = 108 bytes */
