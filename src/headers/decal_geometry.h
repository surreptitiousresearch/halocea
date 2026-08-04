#pragma once
/* decal_geometry — scratch buffer that decal_clip_to_surface fills with the clipped decal polygons
 * for the surfaces a decal touches (DB types_members-confirmed, 22532 bytes). Purely a work area:
 * decal_new_from_collision allocates one on the stack and reads back the emitted vertices/surfaces. */

#include "decal_vertex_internal.h"

typedef struct decal_geometry
{
    decal_vertex_internal decal_vertices[1024];       /* 0x0000 */
    __int16               decal_vertex_count;         /* 0x5000 */
    __int16               decal_surface_vertex_counts[1024]; /* 0x5002 — vertex count per clipped surface */
    __int16               decal_surface_count;        /* 0x5802 */
} decal_geometry;                                     /* 22532 bytes */
