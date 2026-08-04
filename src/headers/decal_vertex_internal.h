#pragma once
/* decal_vertex_internal — one clipped decal vertex accumulated by decal_clip_to_surface
 * inside a decal_geometry scratch buffer (DB types_members-confirmed, 20 bytes). */

#include "real_point3d.h"
#include "real_point2d.h"

typedef struct decal_vertex_internal
{
    real_point3d position; /* 0x00 */
    real_point2d texcoord; /* 0x0C */
} decal_vertex_internal;   /* 20 bytes */
