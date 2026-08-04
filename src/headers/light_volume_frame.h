#pragma once
/* light_volume_frame — one keyframe of a light-volume animation (176 bytes, DB layout). The `*_unused`
 * padding blocks are reproduced at faithful offsets so the float indices used by the interpolator line up. */

#include "real_argb_color.h"

typedef struct light_volume_frame
{
    int             unused1[4];          /* 0x00 */
    float           offset_from_marker;  /* 0x10 */
    float           offset_exponent;     /* 0x14 */
    float           length;              /* 0x18 */
    int             length_unused[8];    /* 0x1C */
    float           radius_hither;       /* 0x3C */
    float           radius_yon;          /* 0x40 */
    float           radius_exponent;     /* 0x44 */
    int             radius_unused[8];    /* 0x48 */
    real_argb_color color_hither;        /* 0x68 */
    real_argb_color color_yon;           /* 0x78 */
    float           color_exponent;      /* 0x88 */
    float           brightness_exponent; /* 0x8C */
    int             color_unused[8];     /* 0x90 */
} light_volume_frame;                    /* 0xB0 */
