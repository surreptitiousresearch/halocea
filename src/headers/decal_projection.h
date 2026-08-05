#pragma once
/* decal_projection — the projected decal basis/extent used to clip a decal onto BSP surfaces
 * (DB types_members-confirmed, 140 bytes). Built by decal_projection_create from a real_matrix4x3
 * basis and a real_rectangle2d extent; consumed by decal_clip_to_surface. */

#include <stdint.h>
#include "real_matrix4x3.h"
#include "real_rectangle2d.h"
#include "real_plane3d.h"
#include "real_point2d.h"
#include "real_vector2d.h"

typedef struct decal_projection
{
    real_matrix4x3   basis;             /* 0x00 */
    real_rectangle2d extent;            /* 0x34 */
    real_plane3d     plane;             /* 0x44 */
    int16_t          axis;              /* 0x54 */
    uint8_t  sign;              /* 0x56 */
    uint8_t  pad_57;            /* 0x57 */ /* padding byte */
    real_point2d     decal_points2d[4]; /* 0x58 */
    real_vector2d    texture_u_axis;    /* 0x78 */
    real_vector2d    texture_v_axis;    /* 0x80 */
    float            texture_scale;     /* 0x88 */
} decal_projection;                     /* 140 bytes */
