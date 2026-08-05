#pragma once
/* _shader_transparent_meter — meter-shader-specific body (220 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "real_rgb_color.h"
#include "tag_reference.h"

typedef struct _shader_transparent_meter
{
    uint16_t        flags;                    /* 0x00 — shader_transparent_meter_flags bits */
    int16_t         type;                     /* 0x02 */
    int             unused1[8];               /* 0x04 */
    tag_reference   map;                      /* 0x24 */
    int             unused2[8];               /* 0x34 */
    real_rgb_color  gradient_min_color;       /* 0x54 */
    real_rgb_color  gradient_max_color;       /* 0x60 */
    real_rgb_color  background_color;         /* 0x6C */
    real_rgb_color  flash_color;              /* 0x78 */
    real_rgb_color  tint_color;               /* 0x84 */
    float           meter_transparency;       /* 0x90 */
    float           background_transparency;  /* 0x94 */
    int             unused3[6];               /* 0x98 */
    int16_t         meter_brightness_source;  /* 0xB0 */
    int16_t         flash_brightness_source;  /* 0xB2 */
    int16_t         value_source;             /* 0xB4 */
    int16_t         gradient_source;          /* 0xB6 */
    int16_t         flash_extension_source;   /* 0xB8 */
    uint16_t        pad;                      /* 0xBA */
    int             unused4[8];               /* 0xBC */
} _shader_transparent_meter;                  /* 0xDC = 220 bytes */
