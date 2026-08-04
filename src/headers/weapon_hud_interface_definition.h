#pragma once
/* weapon_hud_interface_definition — a weapon_hud_interface tag: the full HUD layout for one weapon, plus a
 * chain to a parent HUD interface. Layout from the database (380 bytes). */

#include "tag_reference.h"
#include "tag_block.h"
#include "weapon_flash_state_definition.h"
#include "hud_absolute_placement_definition.h"
#include "icon_hud_element_definition.h"

typedef struct weapon_hud_interface_definition
{
    tag_reference                     parent_hud;                  /* 0x00 */
    weapon_flash_state_definition     flash_cutoffs;               /* 0x10 */
    hud_absolute_placement_definition absolute_placement;          /* 0x3C */
    tag_block                         statics;                     /* 0x60 — weapon_hud_static_element[] */
    tag_block                         meters;                      /* 0x6C — weapon_hud_meter_element[] */
    tag_block                         numbers;                     /* 0x78 — weapon_hud_number_element[] */
    tag_block                         crosshairs;                  /* 0x84 */
    tag_block                         overlays;                    /* 0x90 — weapon_hud_overlays_element[] */
    unsigned int                      valid_crosshair_types_flags; /* 0x9C */
    tag_block                         warning_sounds;              /* 0xA0 */
    tag_block                         screen_effects;              /* 0xAC */
    int                               unused1[33];                 /* 0xB8 */
    icon_hud_element_definition       messaging_icon;              /* 0x13C */
    int                               unused2[12];                 /* 0x14C */
} weapon_hud_interface_definition;                                  /* 380 bytes */
