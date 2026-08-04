#pragma once
/* point_light_color_parameters — the color-transition block of a point_light_definition (0x30 bytes).
 * lower_bound/upper_bound are blended by interpolation_flags/transition_function's normalized t. */

#include "real_argb_color.h"

typedef struct point_light_color_parameters
{
    unsigned int    interpolation_flags; /* 0x00 */
    real_argb_color lower_bound;         /* 0x04 */
    real_argb_color upper_bound;         /* 0x14 */
    int             unused[3];           /* 0x24 */
} point_light_color_parameters; /* 0x30 */
