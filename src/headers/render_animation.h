#pragma once
/* render_animation — runtime animation inputs sampled when evaluating shader texture animations (8 bytes).
 * Layout verbatim from the database. */

#include "real_rgb_color.h"

typedef struct render_animation
{
    const real_rgb_color *colors;  /* 0x00 */
    const float          *values;  /* 0x04 */
} render_animation;
