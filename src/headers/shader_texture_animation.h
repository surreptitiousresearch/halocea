#pragma once
/* shader_texture_animation — the per-axis (u/v/rotation) texture-coordinate animation parameters shared by
 * shader tags (0x38 bytes). Layout verbatim from the database. */

#include <stdint.h>
#include "real_point2d.h"

typedef struct shader_texture_animation
{
    int16_t      u_source;      /* 0x00 — animation input source index (0 = none) */
    int16_t      u_function;    /* 0x02 — periodic function type */
    float        u_period;      /* 0x04 */
    float        u_phase;       /* 0x08 */
    float        u_scale;       /* 0x0C */
    int16_t      v_source;      /* 0x10 */
    int16_t      v_function;    /* 0x12 */
    float        v_period;      /* 0x14 */
    float        v_phase;       /* 0x18 */
    float        v_scale;       /* 0x1C */
    int16_t      r_source;      /* 0x20 — rotation */
    int16_t      r_function;    /* 0x22 */
    float        r_period;      /* 0x24 */
    float        r_phase;       /* 0x28 */
    float        r_scale;       /* 0x2C */
    real_point2d r_center;      /* 0x30 — rotation center */
} shader_texture_animation;
