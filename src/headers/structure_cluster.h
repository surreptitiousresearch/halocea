#pragma once
/* structure_cluster — one cluster block in the structure BSP tag (structures.c).
 * 104 bytes, DB types_members-confirmed layout. */

#include "tag_block.h"

typedef struct structure_cluster
{
    __int16          sky_index;                          /* 0x00 */
    __int16          fog_designator;                     /* 0x02 */
    __int16          background_sound_palette_index;     /* 0x04 */
    __int16          sound_environment_palette_index;    /* 0x06 */
    __int16          weather_palette_index;              /* 0x08 */
    __int16          transitions_to_structure_bsp_index; /* 0x0A */
    __int16          first_runtime_decal_index;          /* 0x0C */
    unsigned __int16 runtime_decal_count;                /* 0x0E */
    int              unused1[6];                         /* 0x10 */
    tag_block        predicted_resources;                /* 0x28 */
    tag_block        subclusters;                        /* 0x34 */
    unsigned __int16 first_lens_flare_marker_index;      /* 0x40 */
    unsigned __int16 lens_flare_marker_count;            /* 0x42 */
    tag_block        surface_indices;                    /* 0x44 */
    tag_block        mirrors;                            /* 0x50 */
    tag_block        portal_indices;                     /* 0x5C */
} structure_cluster;                                     /* 104 bytes */
