#pragma once
/* bsp2d_reference — one per-leaf entry pairing a bsp3d splitting plane with the 2D BSP built on
 * that plane. DB-verified layout (types_members bsp2d_reference, size 8): plane_designator@0,
 * root_index@4, both int. Matches the usage evidence in collision_leaf_test_vector.c,
 * bsp3d_test_pill_recursive.c and bsp3d_test_sphere_recursive.c (8-byte stride from
 * collision_leaf.first_bsp2d_reference_index; +0 read as a plane index whose sign bit encodes
 * plane facing — hence "designator" rather than plain index; +4 passed as the bsp2d root child
 * index to bsp2d_test_*). */

typedef struct bsp2d_reference
{
    int plane_designator; /* 0x00 - plane index; sign bit set = back-facing */
    int root_index;       /* 0x04 - root child of the plane's 2D BSP */
} bsp2d_reference;        /* 8 bytes */
