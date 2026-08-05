#pragma once
/* auxilary_overlay_definition — one entry in the unit HUD's auxilary overlay block (e.g. team-color
 * background/emblem overlays). 132-byte DB layout: a static element plus a team-type index and flags. */

#include <stdint.h>
#include "static_hud_element_definition.h"

typedef struct auxilary_overlay_definition
{
    static_hud_element_definition static_element; /* 0x00 (104) */
    int16_t                       type;           /* 0x68 — _auxilary_overlay_team etc. */
    uint16_t              flags;          /* 0x6A — bit0 = use change/team color */
    int                           unused0[6];     /* 0x6C */
} auxilary_overlay_definition;                     /* 0x84 = 132 bytes */
