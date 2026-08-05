#pragma once
/* lens_flare_reflection — one entry of lens_flare_definition.reflections (128 bytes, DB
 * types_members-confirmed). flags bit0 adds the corona-rotation-function output to rotation_offset,
 * bit1 scales radius by the view-dot distance, bit2 scales radius by (occlusion_fraction+1)*0.5,
 * bit3 gates the stencil-mode-2 draw. */

#include <stdint.h>
#include "real_argb_color.h"

typedef struct lens_flare_reflection
{
    uint16_t flags;                        /* 0x00 */
    int16_t           type;                        /* 0x02 */
    int16_t           bitmap_index;                /* 0x04 */
    uint16_t  pad;                          /* 0x06 */
    int               unused1[5];                   /* 0x08 */
    float             offset;                        /* 0x1C */
    float             rotation_offset;                /* 0x20 */
    int               unused2[1];                      /* 0x24 */
    float             radius_lower_bounds;              /* 0x28 */
    float             radius_upper_bounds;               /* 0x2C */
    int16_t           radius_scale_function;             /* 0x30 */
    uint16_t  radius_pad;                          /* 0x32 */
    float             brightness_lower_bounds;              /* 0x34 */
    float             brightness_upper_bounds;               /* 0x38 */
    int16_t           brightness_scale_function;              /* 0x3C */
    uint16_t  brightness_pad;                          /* 0x3E */
    real_argb_color   tint_color;                                /* 0x40 */
    real_argb_color   animation_color_lower_bound;                /* 0x50 */
    real_argb_color   animation_color_upper_bound;                 /* 0x60 */
    uint16_t  animation_flags;                              /* 0x70 */
    int16_t           animation_function;                            /* 0x72 */
    float             animation_period;                                /* 0x74 */
    float             animation_phase;                                  /* 0x78 */
    int               unused3[1];                                       /* 0x7C */
} lens_flare_reflection; /* 128 bytes */
