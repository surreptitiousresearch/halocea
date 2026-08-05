#pragma once
/* flag_definition — the 'flag' tag definition backing a flag_datum's cloth simulation (96 bytes,
 * DB layout). */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"

typedef struct flag_definition
{
    unsigned int  flags;                 /* 0x00 */
    int16_t       trailing_edge_shape;   /* 0x04 */
    int16_t       trailing_edge_offset;  /* 0x06 */
    int16_t       attached_edge_shape;   /* 0x08 */
    uint16_t pad;                /* 0x0A */
    int16_t       width;                 /* 0x0C */
    int16_t       height;                /* 0x0E */
    float         cell_width_scale;      /* 0x10 */
    float         cell_height_scale;     /* 0x14 */
    tag_reference shader_red;            /* 0x18 */
    tag_reference physics;               /* 0x28 */
    float         wind_noise;            /* 0x38 */
    int           unused2[2];            /* 0x3C */
    tag_reference shader_blue;           /* 0x44 */
    tag_block     attachment_points;     /* 0x54 */
} flag_definition;                        /* 96 bytes */
