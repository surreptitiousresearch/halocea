#pragma once
/* _shader_model — the 400-byte model-shader body carried by the shader_model tag block,
 * following the common 40-byte _shader base (see shader_model.h). Layout DB-verified via
 * types_members; unnamed `unusedN` gaps are the tag's reserved padding runs. */

#include "real_rgb_color.h"
#include "real_argb_color.h"
#include "real_vector2d.h"
#include "tag_reference.h"
#include "shader_texture_animation.h"

typedef struct _shader_model
{
    unsigned __int16 flags;                                        /* 0x000 */
    __int16          type;                                         /* 0x002 */
    int              unused1[3];                                   /* 0x004 */
    float            translucency;                                 /* 0x010 */
    int              unused2[4];                                   /* 0x014 */
    __int16          diffuse_change_color_source;                  /* 0x024 */
    __int16          pad;                                          /* 0x026 */
    int              unused3[7];                                   /* 0x028 */
    unsigned __int16 self_illumination_flags;                      /* 0x044 */
    unsigned __int16 self_illumination_pad;                        /* 0x046 */
    __int16          self_illumination_color_source;               /* 0x048 */
    __int16          self_illumination_animation_function;         /* 0x04A */
    float            self_illumination_animation_period;           /* 0x04C */
    real_rgb_color   self_illumination_animation_color_lower_bound; /* 0x050 */
    real_rgb_color   self_illumination_animation_color_upper_bound; /* 0x05C */
    int              unused4[3];                                   /* 0x068 */
    real_vector2d    map_scale;                                    /* 0x074 */
    tag_reference    base_map;                                     /* 0x07C */
    int              unused5[2];                                   /* 0x08C */
    tag_reference    multipurpose_map;                             /* 0x094 */
    int              unused6[2];                                   /* 0x0A4 */
    __int16          detail_function;                              /* 0x0AC */
    __int16          detail_mask;                                  /* 0x0AE */
    float            detail_map_scale;                             /* 0x0B0 */
    tag_reference    detail_map;                                   /* 0x0B4 */
    float            detail_map_v_scale;                           /* 0x0C4 */
    int              unused7[3];                                   /* 0x0C8 */
    shader_texture_animation animation;                            /* 0x0D4 */
    int              unused8[2];                                   /* 0x10C */
    float            reflection_falloff_distance;                  /* 0x114 */
    float            reflection_cutoff_distance;                   /* 0x118 */
    real_argb_color  reflection_view_perpendicular_color;          /* 0x11C */
    real_argb_color  reflection_view_parallel_color;               /* 0x12C */
    tag_reference    reflection_map;                               /* 0x13C */
    int              unused9[4];                                   /* 0x14C */
    float            reflection_bump_map_scale;                    /* 0x15C */
    tag_reference    reflection_bump_map;                          /* 0x160 */
    int              unused10[8];                                  /* 0x170 */
} _shader_model;                                                   /* 0x190 (400 bytes) */
