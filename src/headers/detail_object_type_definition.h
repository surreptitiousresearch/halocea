#pragma once
/* detail_object_type_definition — one element of a detail_object_collection_definition's
 * `type_definitions` tag_block (96-byte stride). Layout from the database. */

#include "real_rgb_color.h"

typedef struct detail_object_type_definition
{
    char             name[32];              /* 0x00 */
    unsigned char    sequence_index;        /* 0x20 */
    unsigned char    flags;                 /* 0x21 */
    unsigned char    first_frame_index;     /* 0x22 */
    unsigned char    frame_count;           /* 0x23 */
    float            color_override_factor; /* 0x24 */
    int              unused1[2];            /* 0x28 */
    float            near_fade_distance;    /* 0x30 */
    float            far_fade_distance;     /* 0x34 */
    float            size_min;              /* 0x38 */
    float            size_max;              /* 0x3C */
    real_rgb_color   color_min;             /* 0x40 */
    real_rgb_color   color_max;             /* 0x4C */
    unsigned int     color_ambient;         /* 0x58 */
    int              unused2[1];            /* 0x5C */
} detail_object_type_definition; /* 96 bytes */
