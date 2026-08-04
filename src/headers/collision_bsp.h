/* collision_bsp — a leaf-resolving collision BSP: a 3D BSP plus the per-leaf 2D BSPs and the surface
 * table. Partial: bsp3d, leaves and surfaces are modeled (the fields the vector test reads); bsp2d
 * and the edge/vertex tables are reached only inside collision_leaf_test_vector. */
#pragma once

#include "bsp3d.h"
#include "bsp2d.h"
#include "tag_block.h"
#include "real_point3d.h"
#include "collision_surface.h"
#include "collision_edge.h"
#include "collision_vertex.h"

typedef struct collision_bsp
{
    bsp3d     bsp3d;            /* 0x00 */
    tag_block leaves;           /* 0x18 — each 8 bytes; word0 bit0 = double-sided contents */
    tag_block bsp2d_references; /* 0x24 */
    bsp2d     bsp2d;            /* 0x30 */
    tag_block surfaces;         /* 0x3C — each 12 bytes: int plane_designator, byte flags@8,
                                 *        byte breakable_surface_index@9, word material_index@10 */
    tag_block edges;            /* 0x48 */
    tag_block vertices;         /* 0x54 */
} collision_bsp;
