#pragma once
/* game_globals_rasterizer_data — the rasterizer configuration block from the game globals tag (428 bytes). */

#include <stdint.h>
#include "tag_reference.h"
#include "real_rgb_color.h"

typedef struct game_globals_rasterizer_data
{
    tag_reference distance_attenuation;                                     /* 0x000 */
    tag_reference vector_normalization;                                     /* 0x010 */
    tag_reference atmospheric_fog_density;                                  /* 0x020 */
    tag_reference planar_fog_density;                                       /* 0x030 */
    tag_reference linear_corner_fade;                                       /* 0x040 */
    tag_reference active_camouflage_distortion;                             /* 0x050 */
    tag_reference glow;                                                     /* 0x060 */
    int           unused1[15];                                              /* 0x070 */
    tag_reference default_textures[3];                                      /* 0x0AC */
    tag_reference test[4];                                                  /* 0x0DC */
    tag_reference screen_effect_video_scanline_map;                        /* 0x11C */
    tag_reference screen_effect_video_noise_map;                           /* 0x12C */
    int           unused2[13];                                              /* 0x13C */
    uint16_t active_camouflage_flags;                               /* 0x170 */
    uint16_t pad;                                                   /* 0x172 */
    float         active_camouflage_refraction_amount;                      /* 0x174 */
    float         active_camouflage_distance_falloff;                       /* 0x178 */
    real_rgb_color active_camouflage_tint_color;                            /* 0x17C */
    float         active_camouflage_hyper_stealth_refraction_amount;        /* 0x188 */
    float         active_camouflage_hyper_stealth_distance_falloff;         /* 0x18C */
    real_rgb_color active_camouflage_hyper_stealth_tint_color;              /* 0x190 */
    tag_reference distance_attenuation_2d_for_the_pc;                       /* 0x19C */
} game_globals_rasterizer_data;                                            /* 428 bytes (0x1AC) */

extern game_globals_rasterizer_data *global_rasterizer_data;
