#pragma once
/* sky_light — one light entry in a sky tag's lights block (sky.c / render_sky.c).
 * 116 bytes, DB types_members-confirmed layout. */

#include "tag_reference.h"
#include "sky_radiosity_light.h"

typedef struct sky_light
{
    tag_reference       lens_flare;               /* 0x00 — .index @0x0C is the lens-flare effect definition */
    char                lens_flare_marker_name[32];/* 0x10 */
    int                 unused[7];                 /* 0x30 */
    sky_radiosity_light radiosity;                 /* 0x4C — .direction @0x68 is the fallback facing */
} sky_light;                                       /* 116 bytes */
