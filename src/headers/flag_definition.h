#pragma once
/* flag_definition — the 'flag' tag definition backing a flag_datum's cloth simulation (96 bytes,
 * DB layout). */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct flag_definition
{
    unsigned int  flags;                 /* 0x00 */
    __int16       trailing_edge_shape;   /* 0x04 */
    __int16       trailing_edge_offset;  /* 0x06 */
    __int16       attached_edge_shape;   /* 0x08 */
    unsigned __int16 pad;                /* 0x0A */
    __int16       width;                 /* 0x0C */
    __int16       height;                /* 0x0E */
    float         cell_width_scale;      /* 0x10 */
    float         cell_height_scale;     /* 0x14 */
    tag_reference shader_red;            /* 0x18 */
    tag_reference physics;               /* 0x28 */
    float         wind_noise;            /* 0x38 */
    int           unused2[2];            /* 0x3C */
    tag_reference shader_blue;           /* 0x44 */
    tag_block     attachment_points;     /* 0x54 */
} flag_definition;                        /* 96 bytes */
