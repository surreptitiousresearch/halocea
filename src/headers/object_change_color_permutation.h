#pragma once
/* object_change_color_permutation — one element of an object_change_color_definition's permutations
 * tag block (28 bytes). Layout from the database (types_members). */

#include "real_rgb_color.h"

typedef struct object_change_color_permutation
{
    float          weight;             /* 0x00 */
    real_rgb_color color_lower_bound;  /* 0x04 */
    real_rgb_color color_upper_bound;  /* 0x10 */
} object_change_color_permutation;     /* 28 bytes */
