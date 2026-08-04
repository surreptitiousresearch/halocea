#pragma once
/* bsp2d_reference — one per-leaf entry pairing a bsp3d splitting plane with the 2D BSP built on
 * that plane. No DB struct exists for this block; layout is usage-evidenced by
 * collision_leaf_test_vector.c, bsp3d_test_pill_recursive.c and bsp3d_test_sphere_recursive.c
 * (8-byte stride from collision_leaf.first_bsp2d_reference_index; +0 read as a plane index whose
 * sign bit encodes plane facing; +4 passed as the bsp2d root child index to bsp2d_test_*). */

typedef struct bsp2d_reference
{
    int plane_index;      /* 0x00 - sign bit set = back-facing */
    int bsp2d_root_index; /* 0x04 - root child of the plane's 2D BSP */
} bsp2d_reference;        /* 8 bytes */
