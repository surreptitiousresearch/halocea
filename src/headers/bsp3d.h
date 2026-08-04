/* bsp3d — a 3D binary space partition: a tree of nodes, each referencing a splitting plane and two
 * children (front/back). Leaf children are encoded as negative indices. */
#pragma once

#include "tag_block.h"

typedef struct bsp3d
{
    tag_block nodes;  /* 0x00 — each node: int plane_index, int back_child, int front_child */
    tag_block planes; /* 0x0C — real_plane3d each */
} bsp3d;
