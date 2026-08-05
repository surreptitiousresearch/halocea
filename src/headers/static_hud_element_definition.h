#pragma once
/* static_hud_element_definition — a fixed (non-animated-number) HUD icon element: placement +
 * bitmap reference + tint colors + sprite sequence (hud.c). Layout from the database (104 bytes). */

#include <stdint.h>
#include "hud_placement_definition.h"
#include "tag_reference.h"
#include "hud_color_definition.h"
#include "tag_block.h"

typedef struct static_hud_element_definition
{
    hud_placement_definition placement;              /* 0x00 */
    tag_reference            interface_bitmap;        /* 0x24 */
    hud_color_definition      colors;                  /* 0x34 */
    int16_t                   sequence_index;           /* 0x54 */
    int16_t                   pad;                       /* 0x56 */
    tag_block                 multitexture_overlays;     /* 0x58 */
    int                       unused0[1];                 /* 0x64 */
} static_hud_element_definition;                          /* 0x68 (104 bytes) */
