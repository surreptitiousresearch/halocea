#pragma once
/* auxilary_panel_definition — the unit HUD's auxilary overlay panel: a corner anchor plus a tag_block
 * of auxilary_overlay_definition entries. 64-byte DB layout. */

#include "hud_absolute_placement_definition.h"
#include "tag_block.h"

typedef struct auxilary_panel_definition
{
    hud_absolute_placement_definition absolute_placement; /* 0x00 (36) */
    tag_block                         auxilary_overlays;  /* 0x24 — auxilary_overlay_definition[] */
    int                               unused[4];          /* 0x30 */
} auxilary_panel_definition;                              /* 0x40 = 64 bytes */
