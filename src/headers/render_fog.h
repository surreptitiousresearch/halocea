#pragma once
/* render_fog — atmospheric/planar fog parameters latched per render window. */

#include "real_rgb_color.h"
#include "real_plane3d.h"

typedef struct fog_screen fog_screen;

typedef struct render_fog
{
    unsigned __int16 fog_definition_flags;         /* 0x00 */
    unsigned __int16 runtime_flags;                /* 0x02 */
    real_rgb_color   atmospheric_color;            /* 0x04 */
    float            atmospheric_maximum_density;   /* 0x10 */
    float            atmospheric_minimum_distance;  /* 0x14 */
    float            atmospheric_maximum_distance;  /* 0x18 */
    __int16          planar_mode;                  /* 0x1C */
    unsigned char _pad0[2]; /* db-verified padding */
    real_plane3d     plane;                        /* 0x20 */
    real_rgb_color   planar_color;                 /* 0x30 */
    float            planar_maximum_density;        /* 0x3C */
    float            planar_maximum_distance;       /* 0x40 */
    float            planar_maximum_depth;          /* 0x44 */
    const fog_screen *screen;                      /* 0x48 */
    float            screen_external_intensity;     /* 0x4C */
} render_fog;                                       /* 0x50 */
