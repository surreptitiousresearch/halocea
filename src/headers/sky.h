#pragma once
/* sky — scenario sky tag. Complete: the database models this type as exactly these 14 members
 * (208 bytes total; DB types/types_members, size 208). All fields named. */

#include "tag_reference.h"
#include "real_rgb_color.h"
#include "sky_atmospheric_fog.h"
#include "tag_block.h"

typedef struct sky
{
    tag_reference       model;                          /* 0x00 — .index == -1 when no sky model */
    tag_reference       animation_graph;                /* 0x10 */
    int                 unused[6];                      /* 0x20 */
    real_rgb_color      radiosity_indoor_ambient_color; /* 0x38 */
    float               radiosity_indoor_ambient_power; /* 0x44 */
    real_rgb_color      radiosity_outdoor_ambient_color;/* 0x48 */
    float               radiosity_outdoor_ambient_power;/* 0x54 */
    sky_atmospheric_fog outdoor_fog;                    /* 0x58 */
    sky_atmospheric_fog indoor_fog;                     /* 0x78 */
    tag_reference       indoor_fog_plane;               /* 0x98 */
    int                 unused2[1];                     /* 0xA8 */
    tag_block           shader_functions;               /* 0xAC */
    tag_block           animations;                     /* 0xB8 */
    tag_block           lights;                         /* 0xC4 */
} sky;                                                  /* 0xD0 = 208 bytes */
