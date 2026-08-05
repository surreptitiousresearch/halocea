#pragma once
/* meter_hud_element_definition — a HUD meter element's tag definition (104 bytes, DB layout): its shared
 * placement, meter bitmap, gradient/flash/empty colors, alpha shaping, and fade/opacity. */

#include <stdint.h>
#include "hud_placement_definition.h"
#include "tag_reference.h"
#include "tag_block.h"

typedef struct meter_hud_element_definition
{
    hud_placement_definition placement;      /* 0x00 */
    tag_reference    meter_bitmap;           /* 0x24 */
    unsigned int     min_color;              /* 0x34 */
    unsigned int     max_color;              /* 0x38 */
    unsigned int     flash_color;            /* 0x3C */
    unsigned int     empty_color;            /* 0x40 */
    uint8_t  meter_flags;            /* 0x44 */
    uint8_t  minimum_value;          /* 0x45 */
    int16_t          sequence_index;         /* 0x46 */
    uint8_t  alpha_multiplier;       /* 0x48 */
    uint8_t  alpha_bias;             /* 0x49 */
    int16_t          value_scale;            /* 0x4A */
    float            opacity;                /* 0x4C */
    float            fade;                   /* 0x50 */
    unsigned int     disabled_color;         /* 0x54 */
    tag_block        multitexture_overlays;  /* 0x58 */
    int              unused0[1];             /* 0x64 */
} meter_hud_element_definition;              /* 0x68 = 104 bytes */
