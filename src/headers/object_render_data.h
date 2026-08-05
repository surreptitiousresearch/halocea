#pragma once
/* object_render_data — the per-object request block passed to process_rendered_objects describing how to
 * render an object (or, with object_index = -1, a whole environment pass such as shadows). Layout from the
 * database (72 bytes). */

#include <stdint.h>
#include "render_lighting.h"
#include "real_matrix4x3.h"

typedef struct object_render_data
{
    int              object_index;            /* 0x00 */
    render_lighting *lighting;                /* 0x04 */
    uint8_t  shadow;                  /* 0x08 */
    uint8_t  no_planar_fog;           /* 0x09 */
    uint8_t  _pad0A[2];               /* 0x0A */
    real_matrix4x3   shadow_matrix;           /* 0x0C */
    float            shadow_bounding_radius;  /* 0x40 */
    int              rasterizer_bitmap_cookie;/* 0x44 */
} object_render_data;                         /* 0x48 */
