#pragma once
/* render_lighting — the resolved lighting environment applied to a single object when it is rendered:
 * ambient term, up to two distant (directional) lights, up to two point-light references, a reflection tint,
 * and a shadow direction/color. Layout from the database (116 bytes). */

#include <stdint.h>
#include "real_rgb_color.h"
#include "real_argb_color.h"
#include "real_vector3d.h"
#include "render_distant_light.h"

typedef struct render_lighting
{
    real_rgb_color       ambient_color;          /* 0x00 */
    int16_t              distant_light_count;    /* 0x0C */
    uint16_t     pad;                     /* 0x0E */
    render_distant_light distant_lights[2];      /* 0x10 */
    int16_t              point_light_count;      /* 0x40 */
    uint16_t     pad1;                    /* 0x42 */
    int                  point_light_indices[2]; /* 0x44 */
    real_argb_color      reflection_tint_color;  /* 0x4C */
    real_vector3d        shadow_vector;          /* 0x5C */
    real_rgb_color       shadow_color;           /* 0x68 */
} render_lighting;                                /* 116 bytes */
