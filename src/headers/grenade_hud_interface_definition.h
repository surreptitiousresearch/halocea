#pragma once
/* grenade_hud_interface_definition — the 'ghi!' grenade-HUD tag: an icon panel (background), a
 * count panel (icon + numeric count + flash cutoff), a set of state overlays, warning sounds, and a
 * messaging-icon bitmap (hud.c). Layout from the database (504 bytes); unused reflexives left opaque. */

#include "hud_absolute_placement_definition.h"
#include "static_hud_element_definition.h"
#include "grenade_count_panel_definition.h"
#include "weapon_hud_overlay_definition.h"
#include "tag_block.h"
#include "tag_reference.h"

typedef struct grenade_hud_interface_definition
{
    hud_absolute_placement_definition absolute_placement;    /* 0x000 */
    static_hud_element_definition     background;             /* 0x024 */
    grenade_count_panel_definition    grenade_count_panel;      /* 0x08C */
    weapon_hud_overlay_definition     overlays;                  /* 0x14C */
    tag_block                         warning_sounds;             /* 0x168 */
    int                               unused0[17];                 /* 0x174 */
    tag_reference                     messaging_icon_bitmap;        /* 0x1B8 */
    int                               unused2[12];                   /* 0x1C8 */
} grenade_hud_interface_definition;                                  /* 0x1F8 (504 bytes) */
