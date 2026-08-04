#pragma once

/* hud_screen_effect_definition — element of weapon_hud_interface_definition::screen_effects
 * (184 bytes, DB-verified). */

#include "tag_reference.h"
#include "real_rgb_color.h"

typedef struct hud_screen_effect_definition
{
    int              unused1[1];                       /* 0x00 */
    unsigned __int16 mask_flags;                       /* 0x04 */
    unsigned __int16 mask_pad;                         /* 0x06 */
    int              mask_unused[4];                   /* 0x08 */
    tag_reference    mask_fullscreen;                  /* 0x18 */
    tag_reference    mask_splitscreen;                 /* 0x28 */
    int              unused2[2];                       /* 0x38 */
    unsigned __int16 convolution_flags;                /* 0x40 */
    unsigned __int16 convolution_pad;                  /* 0x42 */
    float            convolution_radius_in_bounds[2];  /* 0x44 */
    float            convolution_radius_out_bounds[2]; /* 0x4C */
    int              unused3[6];                        /* 0x54 */
    unsigned __int16 light_enhancement_flags;          /* 0x6C */
    __int16          light_enhancement_script_source;  /* 0x6E */
    float            light_enhancement_intensity;      /* 0x70 */
    int              unused4[6];                        /* 0x74 */
    unsigned __int16 desaturation_flags;               /* 0x8C */
    __int16          desaturation_script_source;       /* 0x8E */
    float            desaturation_intensity;            /* 0x90 */
    real_rgb_color   desaturation_tint;                /* 0x94 */
    int              unused5[6];                        /* 0xA0 */
} hud_screen_effect_definition;                         /* 0xB8 = 184 bytes */
