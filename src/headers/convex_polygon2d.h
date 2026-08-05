#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* convex_polygon2d — a bounded convex polygon in a leaf's 2D plane (516 bytes): a point count followed by up
 * to 64 vertices. Layout recovered from leaf_map_build_leaf_face_for_leaf_on_node (count @0x00 read via lhz,
 * points @0x04, total copy size 0x204). */

#include <stdint.h>
#include "real_point2d.h"

typedef struct convex_polygon2d
{
    int16_t      point_count;   /* 0x00 */
    int16_t      _pad02;        /* 0x02 */
    real_point2d points[64];    /* 0x04 */
} convex_polygon2d;             /* 0x204 */
