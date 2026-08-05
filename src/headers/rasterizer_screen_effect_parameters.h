#pragma once
/* rasterizer_screen_effect_parameters — the live parameter block for the full-screen post-process
 * ("filth"/cinematic screen effect): convolution blur, desaturation filter, and the analog-video
 * overlay. 56 bytes; offsets verified against the database. */

#include <stdint.h>
#include "real_rgb_color.h"

struct bitmap_data;

typedef struct rasterizer_screen_effect_parameters
{
    int16_t              convolution_extra_passes;                       /* 0x00 */
    int16_t              convolution_type;                               /* 0x02 */
    float                convolution_radius;                             /* 0x04 */
    struct bitmap_data  *convolution_mask;                               /* 0x08 */
    float                filter_light_enhancement_intensity;             /* 0x0C */
    float                filter_desaturation_intensity;                  /* 0x10 */
    real_rgb_color       filter_desaturation_tint;                       /* 0x14 */
    uint8_t      filter_desaturation_is_additive;                /* 0x20 */
    uint8_t      filter_light_enhancement_uses_convolution_mask; /* 0x21 */
    uint8_t      filter_desaturation_uses_convolution_mask;      /* 0x22 */
    uint8_t      video_on;                                       /* 0x23 */
    int16_t              video_overbright_mode;                          /* 0x24 */
    unsigned char        _pad26[2];                                      /* 0x26 */
    struct bitmap_data  *video_scanline_map;                            /* 0x28 */
    float                video_noise_intensity;                          /* 0x2C */
    float                video_noise_map_scale;                          /* 0x30 */
    struct bitmap_data  *video_noise_map;                               /* 0x34 */
} rasterizer_screen_effect_parameters;                                   /* 56 bytes */
