#pragma once
/* fog_definition — the 'fog ' tag body (396 bytes). Full DB layout (types_members fog_definition);
   flags bit 1 = "is water". */

#include "real_rgb_color.h"
#include "fog_screen.h"
#include "tag_reference.h"

typedef struct fog_definition
{
    unsigned int   flags;                       /* 0x00 — bit 1: is water */
    float          animation_distance;          /* 0x04 */
    int            animation_unused[19];        /* 0x08 */
    int            unused1[1];                  /* 0x54 */
    float          maximum_density;             /* 0x58 */
    int            unused2[1];                  /* 0x5C */
    float          maximum_distance;            /* 0x60 */
    int            unused3[1];                  /* 0x64 */
    float          maximum_depth;               /* 0x68 */
    int            unused4[2];                  /* 0x6C */
    float          distance_to_water_plane;     /* 0x74 */
    real_rgb_color color;                       /* 0x78 */
    fog_screen     screen;                      /* 0x84 (112 bytes) */
    tag_reference  background_sound;            /* 0xF4 */
    tag_reference  sound_environment;           /* 0x104 */
    int            sound_unused[30];            /* 0x114 */
} fog_definition;                               /* 396 bytes */
