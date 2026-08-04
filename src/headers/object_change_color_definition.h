#pragma once
/* object_change_color_definition — one element of an object definition's change_colors tag block
 * (44 bytes). Layout from the database (types_members). */

#include "real_rgb_color.h"
#include "tag_block.h"

typedef struct object_change_color_definition
{
    __int16        darken_by;          /* 0x00 — exported function index scaling the result, 1-based */
    __int16        scaled_by;          /* 0x02 — exported function index driving interpolation, 1-based */
    unsigned int   scale_flags;        /* 0x04 */
    real_rgb_color color_lower_bound;  /* 0x08 */
    real_rgb_color color_upper_bound;  /* 0x14 */
    tag_block      permutations;       /* 0x20 */
} object_change_color_definition;      /* 44 bytes */
