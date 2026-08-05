#pragma once
/* structure_cluster — one cluster block in the structure BSP tag (structures.c).
 * 104 bytes, DB types_members-confirmed layout. */

#include <stdint.h>
#include "tag_block.h"

typedef struct structure_cluster
{
    int16_t          sky_index;                          /* 0x00 */
    int16_t          fog_designator;                     /* 0x02 */
    int16_t          background_sound_palette_index;     /* 0x04 */
    int16_t          sound_environment_palette_index;    /* 0x06 */
    int16_t          weather_palette_index;              /* 0x08 */
    int16_t          transitions_to_structure_bsp_index; /* 0x0A */
    int16_t          first_runtime_decal_index;          /* 0x0C */
    uint16_t runtime_decal_count;                /* 0x0E */
    int              unused1[6];                         /* 0x10 */
    tag_block        predicted_resources;                /* 0x28 */
    tag_block        subclusters;                        /* 0x34 */
    uint16_t first_lens_flare_marker_index;      /* 0x40 */
    uint16_t lens_flare_marker_count;            /* 0x42 */
    tag_block        surface_indices;                    /* 0x44 */
    tag_block        mirrors;                            /* 0x50 */
    tag_block        portal_indices;                     /* 0x5C */
} structure_cluster;                                     /* 104 bytes */
