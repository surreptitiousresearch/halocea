#pragma once

/* actor_variant_change_colors — one change-color record in an
 * actor_variant_definition's change_colors tag_block (32 bytes). Each record is a
 * lower/upper real_rgb_color pair that a per-object random value interpolates
 * between. DB type actor_variant_change_colors. */

#include "real_rgb_color.h"

typedef struct actor_variant_change_colors
{
    real_rgb_color color_lower_bound; /* 0x00 */
    real_rgb_color color_upper_bound; /* 0x0C */
    unsigned int   unused[2];         /* 0x18 */
} actor_variant_change_colors;        /* 0x20 */
